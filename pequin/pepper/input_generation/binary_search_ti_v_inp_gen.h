

void binary_search_ti_input_gen (mpq_t * input_q, int num_inputs, char *argv[]) {
    for (int i = 0; i < num_inputs - 3; i++) { // A
        mpq_set_ui(input_q[i], 2 * i + (rand() % 2), 1);
    }
    mpq_set_ui(input_q[num_inputs - 3], rand() % (2 * num_inputs + 2), 1); // x
    mpq_set_ui(input_q[num_inputs - 2], 0, 1); // l
    mpq_set_ui(input_q[num_inputs - 1], num_inputs - 3, 1); // r
}