#ifndef MAX_ANIMATION_IMPORT_H
#define MAX_ANIMATION_IMPORT_H


#include <max.h>


class CMaxAnimationImport : public SceneImport
{
public:
  const TCHAR* AuthorName();
  const TCHAR* CopyrightMessage();
  int DoImport(const TCHAR* name, ImpInterface* ii, Interface* i, BOOL suppressPrompts);
  const TCHAR* Ext(int n);
  int ExtCount();
  const TCHAR* LongDesc();
  const TCHAR* OtherMessage1();
  const TCHAR* OtherMessage2();
  const TCHAR* ShortDesc();
  void ShowAbout(HWND window);
  unsigned int Version();

private:

};


#endif
