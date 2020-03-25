int main() {
    int x;
    int y;
    x = 5;
    y = 0;
    switch (x) {
    case 0:
        y = 5;
        break;
    case 10:
        y = y + 1;
        break;
    default:
        y = 23;
    }
    return y;
}
