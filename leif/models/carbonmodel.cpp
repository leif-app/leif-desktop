#include "carbonmodel.h"
#include "carbonprocessor.h"

CarbonModel::CarbonModel(QObject *parent /* = nullptr */):
    QObject{parent}
{
    CarbonProcessor *carbon = CarbonProcessor::Instance();
    if(carbon != nullptr)
    {
        connect(carbon, &CarbonProcessor::lifetimeCarbonChanged, this, &CarbonModel::lifetimeCarbonChanged);
        connect(carbon, &CarbonProcessor::sessionCarbonChanged, this, &CarbonModel::sessionCarbonChanged);
    }
}

int CarbonModel::sessionCarbon() const
{
    CarbonProcessor *carbon = CarbonProcessor::Instance();
    if(carbon != nullptr)
    {
        return carbon->sessionCarbon();
    }

    return 0;
}

int CarbonModel::lifetimeCarbon() const
{
    CarbonProcessor *carbon = CarbonProcessor::Instance();
    if(carbon != nullptr)
    {
        return carbon->lifetimeCarbon();
    }

    return 0;
}

void CarbonModel::clearStats()
{
    CarbonProcessor *carbon = CarbonProcessor::Instance();
    if(carbon != nullptr)
    {
        carbon->clearStats();
    }
}
