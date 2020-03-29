enum TestEnum {
    TEST_VALUE,
    ANOTHER_TEST
};

int main() {
    int x = TEST_VALUE;
    switch (x) {
    case TEST_VALUE:
        return 5;
    case ANOTHER_TEST:
        return 17;
    default:
        return 23;
    }
}
