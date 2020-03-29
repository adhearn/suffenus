struct Point {
    int x;
    int y;
};

int main() {
    struct Point p;
    p.x = 5;
    p.y = 3;
    int z = 12;
    return p.x + p.y;
}
