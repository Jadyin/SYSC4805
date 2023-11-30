
struct timeOfFlightVal
{
    int left;
    bool leftStatus;
    int right;
    bool rightStatus;

    bool overallStatus;
};

void setUpTimeOfFlight();

timeOfFlightVal readTimeOfFlight();
