int main() {
    int y = 1;
    int x;
    for (x = 0; x < 5; x = x + 1) {
        y = y + y;
    }
    return y;
}
