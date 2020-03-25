int main() {
    int x;
    int y;
    y = 0;
    for (x = 0; x < 5; x = x + 1) {
        if (x == 1) {
            break;
        }
        y = y + 1;
    }
    return y;
}
