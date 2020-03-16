void(* reset_mk) (void) = 0;//declare reset function at address 0

bool inv(bool v) {
    if (v) return false;

    return true;
}