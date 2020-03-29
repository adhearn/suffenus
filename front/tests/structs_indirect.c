struct Point {
    int x;
    int y;
};

int main() {
    struct Point p;
    struct Point *pp = &p;
    pp->x = 5;
    pp->y = 3;
    return pp->x + pp->y;
}
