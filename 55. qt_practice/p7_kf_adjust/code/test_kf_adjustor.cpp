#include "kf_adjustor.h"

int main()
{
    KFAdjustor adjustor;
    adjustor.showKFGlobalMap();
    adjustor.viewer->spin();
}