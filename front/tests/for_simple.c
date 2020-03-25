int main() {
    int y = 1;
    for (int x = 0; x < 5; x = x + 1) {
        y = y + y;
    }
    return y;
}
