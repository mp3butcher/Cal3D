#include "TestPrologue.h"


const char* animationText =
"<HEADER MAGIC=\"XAF\" VERSION=\"919\" />\n"
"<ANIMATION NUMTRACKS=\"1\" DURATION=\"40\">\n"
"    <TRACK BONEID=\"0\" TRANSLATIONREQUIRED=\"0\" TRANSLATIONISDYNAMIC=\"0\" HIGHRANGEREQUIRED=\"1\" NUMKEYFRAMES=\"2\">\n"
"        <KEYFRAME TIME=\"0\">\n"
"            <ROTATION>0.5 0.5 0.5 -0.5</ROTATION>\n"
"        </KEYFRAME>\n"
"        <KEYFRAME TIME=\"40\">\n"
"            <ROTATION>0.5 0.5 0.5 -0.5</ROTATION>\n"
"        </KEYFRAME>\n"
"    </TRACK>\n"
"</ANIMATION>\n"
;


TEST(LoadSimpleXmlAnimation) {

  // Load animation.

  CalCoreAnimation* anim = CalLoader::loadXmlCoreAnimation(animationText, 0);
  CHECK(anim);

  // Check animation.

  CHECK_EQUAL(anim->numCoreTracks(), 1);
  CHECK_EQUAL(anim->getDuration(), 40);

  // Check tracks.

  CalCoreTrack* track1 = anim->nthCoreTrack(0);
  CalCoreTrack* track2 = anim->getCoreTrack(/*boneid*/ 0);
  CHECK(track1);
  CHECK_EQUAL(track1, track2);

  CHECK_EQUAL(track1->getTranslationRequired(), false);
  CHECK_EQUAL(track1->getTranslationIsDynamic(), false);
  CHECK_EQUAL(track1->getHighRangeRequired(), true);
  CHECK_EQUAL(track1->getCoreKeyframeCount(), 2);

  // Save the animation back to an XML file.

  std::ostringstream ss;
  CalSaver::saveXmlCoreAnimation(ss, anim);

  CHECK_EQUAL(ss.str(), animationText);

  anim->destroy();
  delete anim;
}


#if 0
TEST(Simple) {
  CalLoader loader;
  CalCoreAnimation* ca1 = loader.loadCoreAnimation();
  CalCoreAnimation* ca2 = loader.loadCoreAnimation();

  CHECK(ca1);
  CHECK(ca2);
  CHECK_EQUAL(*ca1, *ca2);

  ca1->Destroy();
  delete ca1;

  ca2->Destroy();
  delete ca2;
}
#endif


TEST(CalVectorFromDataSrc) {
  char buf[7];
  CalBufferSource bs(buf, sizeof(buf)/sizeof(char));

  CalVector vec;
  CHECK_EQUAL(false, CalVectorFromDataSrc(bs, &vec));
}
