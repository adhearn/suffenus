int main() {
    int x = 0;
    int y = 0;
    while (x < 5) {
        if (x == 1) {
            break;
        }
        y = y + 1;
        x = x + 1;
    }
    return y;
}
