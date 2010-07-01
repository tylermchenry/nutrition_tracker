#include "about.h"
#include "version.h"

About::About(QWidget *parent)
    : QDialog(parent)
{
  ui.setupUi(this);

  ui.lblVersion->setText(QString("Version ") + NUTRITIONTRACKER_VERSION);
  ui.lblGitInfo->setText(QString("Built from git revision ") + NUTRITIONTRACKER_GIT_COMMIT);
  ui.lblBuildInfo->setText(QString("Built on ") + NUTRITIONTRACKER_BUILD_TIMESTAMP);

  connect(ui.btnOK, SIGNAL(clicked()), this, SLOT(accept()));
}

About::~About()
{

}
