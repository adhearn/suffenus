int main() {
    int x = 0;
    int y = 1;
    while (x < 5) {
        y = y + y;
        x = x + 1;
    }
    return y;
}
