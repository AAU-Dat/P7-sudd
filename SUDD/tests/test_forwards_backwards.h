#ifndef TEST_FORWARD_BACKWARDS_H
#define TEST_FORWARD_BACKWARDS_H

#include <check.h>
#include <unistd.h>
#include <check.h>
#include <stdio.h>

#include "cudd.h"
#include "helper.h"

#include "../src/forwards_backwards.h"
#include "../src/conversion.h"

Suite* forwards_backwards_suite(void);

#endif