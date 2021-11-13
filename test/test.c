#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include "../src/config.c"
#include "../src/missile.h"
#include "../src/robot.c"

static void check_robots(void **state) {

    robot robots[NUMBER_ROBOTS];
    create_robots(robots);
    for (int i = 0; i < NUMBER_ROBOTS; i++)
    {      
        assert_true(robots[i].id==i);
        assert_true(robots[i].posX<WIDTH);
        assert_true(robots[i].posY<HEIGHT);
    }
}

static void check_is_dead(void **state){
    robot robots[NUMBER_ROBOTS];
    create_robots(robots);
    robots[0].life = 0;
    robots[1].state = DEAD;
    assert_true(is_dead(&robots[0]));
    assert_true(is_dead(&robots[1]));
}

static void check_distance(void **state){
    assert_int_equal(distance(1,2,5,5), 5);
    assert_int_equal(distance(1,2,7,6), 7);
    assert_int_equal(distance(1,1,7,-7), 10);
    assert_int_equal(distance(13,2,7,10), 10);
}

static void check_angle(void **state){
    assert_int_equal(angle(1,2,5,5),36);
    assert_int_equal(angle(1,2,7,6),33);
    assert_int_equal(angle(1,1,7,-7),-52);
    assert_int_equal(angle(13,2,7,10),126);
} 

static void check_engine(void **state){
    robot robots[NUMBER_ROBOTS];
    create_robots(robots);
    robots[0].posY = 200;
    robots[0].posX = 800;
 
    robots[1].posY = 200;    
    robots[1].posX = 200;   

    engine(&robots[0], 45, 100);
    engine(&robots[1], 75, 100);
    update_robots(robots);

    assert_int_equal(robots[0].posX,807);
    assert_int_equal(robots[0].posY,207);
    
    assert_int_equal(robots[1].posX,202);
    assert_int_equal(robots[1].posY,209);
}


static void check_poke_peek(void **state){
    int address = 0;
    int value = 2;
    robot robots[NUMBER_ROBOTS];
    create_robots(robots);
    poke(&robots[0], robots[0].memory[address], value);
    assert_int_equal(value, peek(&robots[0], address));
}

static void check_shoot(void **state){
    robot robots[NUMBER_ROBOTS];
    create_robots(robots);
    shoot(&robots[0],45,100);
    assert_int_equal(robots[0].missiles[0].state, ACTIVE);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(check_robots),
        cmocka_unit_test(check_is_dead),
        cmocka_unit_test(check_distance), 
        cmocka_unit_test(check_angle),
        cmocka_unit_test(check_engine),
        cmocka_unit_test(check_poke_peek),
        cmocka_unit_test(check_shoot)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
