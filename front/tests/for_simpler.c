int main() {
    int y = 1;
    for (int x = 0; x < 5;) {
        y = y + y;
        x = x + 1;
    }
    return y;
}
