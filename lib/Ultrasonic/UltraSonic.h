struct ultraSonicVal
{
    int left;
    bool leftStatus;
    int right;
    bool rightStatus;

    bool overallStatus;
};

void setUpUltraSonic();
ultraSonicVal readUltraSonic();
