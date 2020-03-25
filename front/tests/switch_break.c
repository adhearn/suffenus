int main() {
    int x;
    int y;
    x = 5;
    switch (x) {
    case 0: {
        y = 5;
        break;
    }
    case 5: {
        y = 42;
        break;
    }
    case 10: {
        y = 23;
        break;
    }
    }

    return y;
}
