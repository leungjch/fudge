class Vec3D {
    public:
        double x;
        double y;
        double z;
        Vec3D(double x, double y, double z);
        static double distance(Vec3D v1, Vec3D v2);
        static Vec3D zero();
        static Vec3D random(double range);
};