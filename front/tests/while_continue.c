int main() {
    int x = 0;
    int y = 0;
    while (x < 5) {
        x = x + 1;
        if (x == 1) {
            continue;
        }
        y = y + 1;
    }
    return y;
}
