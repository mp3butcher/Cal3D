/*
www.sourceforge.net/projects/tinyxml
Original code copyright (c) 2004 Eric Ries (tinybind-eric@sneakemail.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/


#ifndef TINYBIND_INCLUDED
#define TINYBIND_INCLUDED

#include "tinyxml.h"

#include <vector>
#include <list>
#include <sstream>

class IdentityBase
{
 public:
};

template< class T >
class Identity : public IdentityBase
{
 public:
  typedef T type;

};




enum MemberSerializeFlags {
  MemberSerializeFlagsNone = 0,
  MemberOptional = 1,
};

class Tag
{
 public:
  // support up to 3 tags
  char const * tag_[3];


  Tag( char const * tagOne = NULL ) {
    tag_[0] = tagOne;
    tag_[1] = NULL;
    tag_[2] = NULL;
  }
  Tag( char const * tagOne, char const * tagTwo ) {
    tag_[0] = tagOne;
    tag_[1] = tagTwo;
    tag_[2] = NULL;
  }
  Tag( char const * tagOne, char const * tagTwo, char const * tagThree ) {
    tag_[0] = tagOne;
    tag_[1] = tagTwo;
    tag_[2] = tagThree;
  }
};

struct SerializeParams {
  Tag tag_;
};

template<class T>
class TiXmlBinding
{
 public:
    virtual bool fromXml( cal3d::TiXmlElement const & elem, T * data, SerializeParams const & ) const = 0;  
  virtual bool intoXml(cal3d::TiXmlElement * elem, T const & data, SerializeParams const & ) const = 0;
};

template<class T>
bool
BindToXml(cal3d::TiXmlElement * elemOut, T const & dataIn )
{
  TiXmlBinding<T> const * binding = GetTiXmlBinding( dataIn, Identity<T>() );
  SerializeParams params;
  return binding->intoXml( elemOut, dataIn, params );
}

template<class T>
bool
BindFromXml(cal3d::TiXmlElement const & elemIn, T * dataOut )
{
  TiXmlBinding<T> const * binding = GetTiXmlBinding( *dataOut, Identity<T>() );
  SerializeParams params;
  return binding->fromXml( elemIn, dataOut, params );
}


template<class T>
class IMemberHolder
{
 public:
  MemberSerializeFlags flags_;
  Tag tag_;
  SerializeParams params_;

  void setFlags( MemberSerializeFlags f ) {
    flags_ = f;
  }

  SerializeParams const & params() {
    params_.tag_ = tag_;
    return params_;
  }
  
  virtual char const * tag( int which = 0) { return tag_.tag_[which]; }
  
  virtual bool fromXml(cal3d::TiXmlElement const &, T * ) = 0;
  virtual bool intoXml(cal3d::TiXmlElement *, T const * ) = 0;
  virtual bool isAttributeMember() = 0;

  
};



template<class T, class MT>
class IMemberValuePolicy
{
 public:
  virtual MT const & getMemberValue( T const * thisPtr ) = 0;
  virtual void setMemberValue( T * thisPtr, MT const & mv ) = 0;
};


template<class T, class MT>
  class MemberFuncHolder : public IMemberValuePolicy<T, MT>
{
 public:
  MT (T::*getter_)();
  void (T::*setter_)(MT);

  virtual MT const & getMemberValue( T const * thisPtr ) {
    static MT mv;
    mv = (const_cast<T*>(thisPtr)->*getter_)();
    return mv;
  }

  virtual void setMemberValue( T * thisPtr, MT const & mv ) {
    (thisPtr->*setter_)(mv);
  }

};

template<class T, class MT>
  class MemberFuncHolderConstRef  : public IMemberValuePolicy<T, MT>
{
 public:
  MT const & (T::*getter_)();
  void (T::*setter_)(MT const &);

  virtual MT const & getMemberValue( T const * thisPtr ) {
    return (thisPtr->*getter_)();
  }

  virtual void setMemberValue( T * thisPtr, MT const & mv ) {
    (thisPtr->*setter_)(mv);
  }
};

template<class T, class MT>
  class MemberPtrHolder : public IMemberValuePolicy<T, MT>
{
 public:
  MT T::*memberPtr_;
  virtual MT const & getMemberValue( T const * thisPtr ) { return thisPtr->*memberPtr_; }
  virtual void setMemberValue( T * thisPtr, MT const & mv ) { 
    // by casting away const here, we can support member pointers to arrays
    //assert(false);
    //thisPtr->*memberPtr_ = const_cast<MT &>(mv); 
  }
};

#if 0
template<class T, class MT>
class MemberArrayHolder : public IMemberValuePolicy<T, MT>
{
public:
  MT T::*memberPtr_;
  size_t len_;

}
#endif


template<class T, class MT>
  class MemberRefFuncHolder : public IMemberValuePolicy<T, MT>
{
 public:
  MT & (T::*memberRefFunc_)();
  virtual MT const & getMemberValue( T const * thisPtr ) { return (const_cast<T*>(thisPtr)->*memberRefFunc_)(); }
  virtual void setMemberValue( T * thisPtr, MT const & mv ) {
    (thisPtr->*memberRefFunc_)() = mv;
  }
};


template<class T, class MT>
  class FromXmlElement : public IMemberHolder<T>
{
 public:
  IMemberValuePolicy<T, MT> * mvPolicy_;
  FromXmlElement( IMemberValuePolicy<T, MT> * mvPolicy )
  {
    mvPolicy_ = mvPolicy;
  }
  
  virtual bool fromXml(cal3d::TiXmlElement const & elem, T * thisPtr)
  {
    MT & mv = const_cast<MT &>(mvPolicy_->getMemberValue(thisPtr));
    TiXmlBinding<MT> const * binding = GetTiXmlBinding( mv,  Identity<MT>()  );
    if( binding->fromXml(elem, &mv, IMemberHolder<T>::params()) ) {
      mvPolicy_->setMemberValue(thisPtr, mv);
      return true;
    } else {
      return false;
    }
  }

  virtual bool intoXml(cal3d::TiXmlElement * elem, T const * thisPtr)
  {
    MT const & mv = mvPolicy_->getMemberValue(thisPtr);
    TiXmlBinding<MT> const * binding = GetTiXmlBinding( mv,  Identity<MT>() );
    std::string oldValue = elem->Value();
    elem->SetValue( IMemberHolder<T>::tag() );
    bool ret = binding->intoXml( elem, mv, IMemberHolder<T>::params() );
    elem->SetValue( oldValue );
    return ret;
  }

  virtual bool isAttributeMember() { return true; }
};


template<class T, class MT, class FromXmlPolicy, class MemberValuePolicy>
  class MemberHolder
{
 public:
  FromXmlPolicy xmlPolicy_;
  MemberValuePolicy mvPolicy_;

  MemberHolder()
    : xmlPolicy_((IMemberValuePolicy<T, MT> *)&mvPolicy_)
  {
  }
};



template<class T, class MT>
  class FromXmlChildElement : public IMemberHolder<T>
{
 public:
  IMemberValuePolicy<T, MT> * mvPolicy_;
  FromXmlChildElement( IMemberValuePolicy<T, MT> * mvPolicy )
  {
    mvPolicy_ = mvPolicy;
  }
  
  virtual bool fromXml(cal3d::TiXmlElement const & elem, T * thisPtr)
  {
    if( !stricmp(elem.Value(), IMemberHolder<T>::tag()) ) {
      MT mv;
      TiXmlBinding<MT> const * binding = GetTiXmlBinding( mv,  Identity<MT>()  );
      if( binding->fromXml(elem, &mv, IMemberHolder<T>::params()) ) {
        mvPolicy_->setMemberValue(thisPtr, mv);
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }

  virtual bool intoXml(cal3d::TiXmlElement * elem, T const * thisPtr)
  {
    MT const & mv = mvPolicy_->getMemberValue(thisPtr);
   cal3d::TiXmlElement child(IMemberHolder<T>::tag());
    TiXmlBinding<MT> const * binding = GetTiXmlBinding( mv, Identity<MT>()  );
    if( binding->intoXml( &child, mv, IMemberHolder<T>::params() ) ) {
      elem->InsertEndChild(child);
      return true;
    } else {
      return false;
    }
  }

  virtual bool isAttributeMember() { return false; }
};


template<class T, class MT>
  class FromXmlAttribute  : public IMemberHolder<T>
{
 public:
  IMemberValuePolicy<T, MT> * mvPolicy_;
  FromXmlAttribute( IMemberValuePolicy<T, MT> * mvPolicy )
  {
    mvPolicy_ = mvPolicy;
  }

  virtual bool fromXml(cal3d::TiXmlElement const & elem, T * thisPtr)
  {
    MT mv;
    const char * attributeValue = elem.Attribute( IMemberHolder<T>::tag() );
    if( attributeValue && *attributeValue ) {
      ConvertFromString( attributeValue, &mv );
      mvPolicy_->setMemberValue(thisPtr, mv);
      return true;
    } else {
      return false;
    }
  }

  virtual bool intoXml(cal3d::TiXmlElement * elem, T const * thisPtr)
  {
    MT const & mv = mvPolicy_->getMemberValue(thisPtr);
    char const * attributeValue = ConvertToString( mv );
    elem->SetAttribute( IMemberHolder<T>::tag(), attributeValue );
    return true;
  }

  virtual bool isAttributeMember() { return true; }
};

template<class T, class MT>
  IMemberHolder<T> * Member(  MT T::*mp )
  {
    typedef FromXmlChildElement<T, MT> XmlPolicy;
    typedef MemberPtrHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * Member(MT & (T::*mp)() )
  {
    typedef FromXmlChildElement<T, MT> XmlPolicy;
    typedef MemberRefFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberRefFunc_ = mp;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * Member(  MT (T::*getter)(), void (T::*setter)(MT) )
  {
    typedef FromXmlChildElement<T, MT> XmlPolicy;
    typedef MemberFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * Member(  MT (T::*getter)()const, void (T::*setter)(MT) )
  {
    typedef FromXmlChildElement<T, MT> XmlPolicy;
    typedef MemberFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = (MT (T::*)())getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * Member( 
      MT const & (T::*getter)(),
      void (T::*setter)(MT const &))
  {
    typedef FromXmlChildElement<T, MT> XmlPolicy;
    typedef MemberFuncHolderConstRef<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

// BEGIN ATTRIBUTE MAKERS
  template<class T, class MT>
    IMemberHolder<T> * MemberAttribute(  MT T::*mp )
  {
    typedef FromXmlAttribute<T, MT> XmlPolicy;
    typedef MemberPtrHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
  }

#if 0
  template<class T, class MT>
    IMemberHolder<T> * MemberAttribute(MT T::*mp, size_t len)
  {
    typedef FromXmlAttribute<T, MT> XmlPolicy;
    typedef MemberArrayHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    mph->mvPolicy_.len = len;
    return &mph->xmlPolicy_;
  }
#endif

  template<class T, class MT>
    IMemberHolder<T> * MemberAttribute(  MT (T::*getter)(), void (T::*setter)(MT) )
  {
    typedef FromXmlAttribute<T, MT> XmlPolicy;
    typedef MemberFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * MemberAttribute(  MT (T::*getter)() const, void (T::*setter)(MT) )
  {
    typedef FromXmlAttribute<T, MT> XmlPolicy;
    typedef MemberFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = (MT (T::*)())getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * MemberAttribute( 
      MT const & (T::*getter)(),
      void (T::*setter)(MT const &))
  {
    typedef FromXmlAttribute<T, MT> XmlPolicy;
    typedef MemberFuncHolderConstRef<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }
// END ATTRIBUTE MAKERS

// BEGIN PEER MAKERS
  template<class T, class MT>
    IMemberHolder<T> * MemberPeer(  MT T::*mp )
  {
    typedef FromXmlElement<T, MT> XmlPolicy;
    typedef MemberPtrHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberPtr_ = mp;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * MemberPeer(  MT (T::*getter)(), void (T::*setter)(MT) )
  {
    typedef FromXmlElement<T, MT> XmlPolicy;
    typedef MemberFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * MemberPeer( 
      MT const & (T::*getter)(),
      void (T::*setter)(MT const &))
  {
    typedef FromXmlElement<T, MT> XmlPolicy;
    typedef MemberFuncHolderConstRef<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.getter_ = getter;
    mph->mvPolicy_.setter_ = setter;
    return &mph->xmlPolicy_;
  }

  template<class T, class MT>
    IMemberHolder<T> * MemberPeer(MT & (T::*mp)() )
  {
    typedef FromXmlElement<T, MT> XmlPolicy;
    typedef MemberRefFuncHolder<T, MT> MemberValuePolicy;
    typedef MemberHolder<T, MT, XmlPolicy, MemberValuePolicy> MH_Type;
    MH_Type * mph = new MH_Type();
    mph->mvPolicy_.memberRefFunc_ = mp;
    return &mph->xmlPolicy_;
  }

// END PEER MAKERS

template<class T>
class MemberTiXmlBinding : public TiXmlBinding<T>
{
 private:
  std::vector<IMemberHolder<T> *> members_;
  
 public:
  bool empty() const
  {
    return members_.empty();
  }

  IMemberHolder<T> * AddMember( Tag tag, IMemberHolder<T> * mph )
  {
    mph->flags_ = MemberSerializeFlagsNone;
    mph->tag_ = tag;
    members_.push_back( mph );
    return mph;
  }

  virtual bool fromXml(cal3d::TiXmlElement const & elem, T * data, SerializeParams const & params ) const 
  {
   cal3d::TiXmlElement const * child = elem.FirstChildElement();
    for( size_t i = 0; i < members_.size(); i++ ) {
      IMemberHolder<T> * mph = members_[i];
      bool error = false;
      
      bool ret;
      if( mph->isAttributeMember() ) {
        ret = mph->fromXml( elem, data );
      } else {
        if( !child ) {
          return false;
        }
        ret = mph->fromXml( *child, data );
      }
      error = !ret;
      if( mph->isAttributeMember() ) {
      } else if( !error ) {
        child = child->NextSiblingElement();
      }
      
      if( error ) {
        if( mph->isAttributeMember() ){
          // no problem
          continue;
        } else {
          // error
          return false;
        }
      }
    }
    return true;
  }
  
  virtual bool intoXml(cal3d::TiXmlElement * elem, T const & data, SerializeParams const & ) const
  {
    for( size_t i = 0; i < members_.size(); i++ ) {
      IMemberHolder<T> * mph = members_[i];
      mph->intoXml( elem, &data );
    }
    return true;
  }
};

template<class T>
char const *
ConvertToString( T const & t );

template<class T>
void
ConvertFromString( char const * strIn, T * dataOut );


template<class T>
class GenericTiXmlBinding : public TiXmlBinding<T>
{
 public:
  virtual bool fromXml(cal3d::TiXmlElement const & elem, T * data, SerializeParams const & ) const 
  {
    cal3d::TiXmlNode * node = elem.FirstChild();
    cal3d::TiXmlText * nodedata = node->ToText();
    ConvertFromString( nodedata->Value(), data );
    return true;
  }
  
  virtual bool intoXml(cal3d::TiXmlElement * elem, T const & data, SerializeParams const & ) const
  {
    cal3d::TiXmlText textData( ConvertToString( data ) );
    elem->InsertEndChild( textData );
    return true;
  }
};

template<class T, class VecT>
class StlContainerTiXmlBinding : public TiXmlBinding<VecT>
{
 public:

  char const * subTag_;
  bool useSubTag_;
  char const * sizeAttributeName_;
  StlContainerTiXmlBinding(bool useSubTag, char const * st = NULL, char const * sizeAttributeName = NULL)
    :subTag_(st), useSubTag_(useSubTag), sizeAttributeName_(sizeAttributeName)
    {
    }
    
    virtual bool fromXml(cal3d::TiXmlElement const & elem, VecT * data, SerializeParams const & params ) const 
    {
      data->clear();
     cal3d::TiXmlElement const * child;
      child = elem.FirstChildElement();
      if( sizeAttributeName_ ) {
        int sz = 0;
        ConvertFromString( elem.Attribute(sizeAttributeName_), &sz );
        if( sz ) {
          //data->reserve(sz);
        }
      }
      while(child) {
        T * value = new T();
        TiXmlBinding<T> const * binding = GetTiXmlBinding( *value,  Identity<T>()  );
        bool ret = binding->fromXml( *child, value, params );
        data->push_back(*value);
        if( ! ret ) {
          return false;
        }
        child = child->NextSiblingElement();
      }
      return true;
    }
    
    virtual bool intoXml(cal3d::TiXmlElement * elem, VecT const & data, SerializeParams const & params ) const
    {
      if( sizeAttributeName_ ) {
        elem->SetAttribute( sizeAttributeName_, ConvertToString( data.size() ) );
      }
      for( typename VecT::const_iterator i = data.begin(); i != data.end(); i++ ) {
        T const & value = *i;
        TiXmlBinding<T> const * binding = GetTiXmlBinding( value,  Identity<T>()  );
        char const * tag;
        if( useSubTag_ ) {
          tag = subTag_;
        } else {
          tag = elem->Value();
        }
       cal3d::TiXmlElement child(tag);
        if( ! binding->intoXml( &child, value, params ) ) {
          return false;
        }
        elem->InsertEndChild(child);
      }
      return true;
    }
  };

template<class T, class VecT>
class StlContainerPtrBinding : public TiXmlBinding<VecT>
{
 public:
    
  char const * subTag_;
  bool useSubTag_;
  char const * sizeAttributeName_;
  StlContainerPtrBinding(bool useSubTag, char const * st = NULL, char const * sizeAttributeName = NULL)
    :subTag_(st), useSubTag_(useSubTag), sizeAttributeName_(sizeAttributeName)
    {
    }

    virtual bool fromXml(cal3d::TiXmlElement const & elem, VecT * data, SerializeParams const & params ) const 
    {
      data->clear();
     cal3d::TiXmlElement const * child;
      child = elem.FirstChildElement();
      if( sizeAttributeName_ ) {
        int sz = 0;
        ConvertFromString( elem.Attribute(sizeAttributeName_), &sz );
        if( sz ) {
          //data->reserve(sz);
        }
      }
      while(child) {
        T * value = new T();
        TiXmlBinding<T> const * binding = GetTiXmlBinding( *value,  Identity<T>()  );
        bool ret = binding->fromXml( *child, value, params );
        data->push_back(value);
        if( ! ret ) {
          return false;
        }
        child = child->NextSiblingElement();
      }
      return true;
    }
    
    virtual bool intoXml(cal3d::TiXmlElement * elem, VecT const & data, SerializeParams const & params ) const
    {
      if( sizeAttributeName_ ) {
        elem->SetAttribute( sizeAttributeName_, ConvertToString( data.size() ) );
      }
      for( typename VecT::const_iterator i = data.begin(); i != data.end(); i++ ) {
        T const * value = *i;
        if( ! value ) {
          continue;
        }
        TiXmlBinding<T> const * binding = GetTiXmlBinding( *value,  Identity<T>()  );
        char const * tag;
        if( useSubTag_ ) {
          tag = subTag_;
        } else {
          tag = elem->Value();
        }
       cal3d::TiXmlElement child(tag);
        if( ! binding->intoXml( &child, *value, params ) ) {
          return false;
        }
        elem->InsertEndChild(child);
      }
      return true;
    }
  };

template<class T>
TiXmlBinding<T> const *
GetTiXmlBinding( T const &, IdentityBase  );


TiXmlBinding<float> const *
GetTiXmlBinding( float const &, IdentityBase  );
TiXmlBinding<double> const *
GetTiXmlBinding( double const &, IdentityBase  );
TiXmlBinding<int> const *
GetTiXmlBinding( int const &, IdentityBase  );
TiXmlBinding<char const *> const *
GetTiXmlBinding( char const * const &, IdentityBase  );
TiXmlBinding<std::string> const *
GetTiXmlBinding( std::string const &, IdentityBase  );

template<class T, class VecT>
TiXmlBinding<VecT> const *
GetTiXmlBinding( std::vector<T> const &, Identity<VecT>  )
{
  static StlContainerTiXmlBinding<T, VecT> binding(false);
  return &binding;
}

template<class T, class VecT>
TiXmlBinding<VecT> const *
GetTiXmlBinding( std::list<T> const &, Identity<VecT>  )
{
  static StlContainerTiXmlBinding<T, VecT> binding(false);
  return &binding;
}


#endif
