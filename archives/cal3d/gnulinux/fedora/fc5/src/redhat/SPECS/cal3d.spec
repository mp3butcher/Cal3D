Name:           cal3d
Version:        0.11
Release:        rc2.1%{?dist}
Summary:        Skeletal based 3-D character animation library
Group:          Development/Libraries
License:        LGPL
URL:            http://gna.org/projects/cal3d
Source0:        http://download.gna.org/cal3d/sources/%{name}-%{version}rc2.tar.bz2
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires:  doxygen docbook-utils
BuildRequires:  autoconf automake libtool

%description
Cal3D is a skeletal based 3-D character animation library written in C++
in a platform-/graphic API-independent way.

%package devel
Summary:        Header files, libraries and development documentation for Cal3D
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}
Requires:       pkgconfig

%description devel
This package contains the header files, libraries and documentation
for Cal3D.

%package doc
Summary:        Documentation files for Cal3D
Group:          Documentation
Requires:       %{name} = %{version}-%{release}

%description doc
This package contains modeling documention and a users guide for Cal3D.

%prep
%setup -q -n %{name}-%{version}rc2
autoreconf --force --install

%build
%configure
%{__make} %{?_smp_mflags}
( cd docs && %{__make} doc-guide && %{__make} doc-api )

%install
%{__rm} -rf %{buildroot}
%{__make} install DESTDIR=%{buildroot}

%check
%{__make} %{?_smp_mflags} check

%clean
%{__rm} -rf %{buildroot}

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%doc AUTHORS ChangeLog COPYING README TODO
%{_bindir}/cal3d_converter
%{_mandir}/man1/cal3d_converter.1.gz
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,-)
%doc docs/api/html/*
%{_includedir}/%{name}
%{_libdir}/pkgconfig/%{name}.pc
%{_libdir}/*.so
%exclude %{_libdir}/*.la

%files doc
%defattr(-,root,root,-)
%doc docs/guide docs/modeling

%changelog
* Thu Jun 22 2006 Christohper Stone <chris.stone@gmail.com> 0.11-rc2.1
- Update Source0 URL
- Remove autoconf patch
- Add cal3d_converter to %%files

* Tue Jun 13 2006 Christopher Stone <chris.stone@gmail.com> 0.10.0-4
- Add libtool to BuildRequires

* Mon Jun 12 2006 Christopher Stone <chris.stone@gmail.com> 0.10.0-3
- Update URL
- Shortened summary description
- Removed %%{name} from devel/doc description and summary
- Add pkgconfig to Requires for devel
- Remove %%makeinstall
- Add %%check section

* Sun May 21 2006 Christopher Stone <chris.stone@gmail.com> 0.10.0-2
- Move some devel documentation into doc package

* Sat May 20 2006 Christopher Stone <chris.stone@gmail.com> 0.10.0-1
- Initial RPM release
