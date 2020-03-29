enum TestEnum {
    TEST_VALUE,
    ANOTHER_TEST,
    WITH_VALUE = 5,
    FINAL
};

int main() {
    int x = FINAL;
    switch (x) {
    case TEST_VALUE:
        return 1;
    case ANOTHER_TEST:
        return 2;
    case WITH_VALUE:
        return WITH_VALUE;
    case FINAL:
        return FINAL;
    default:
        return 0;
    }
}
