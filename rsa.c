#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

// computes a*b  mod n without overflow
uint32_t modmul(uint32_t a, uint32_t b, uint32_t n){
    uint32_t sum = 0;
    for (int i = 0; i < 32; i++){
        if (a & 1){
            sum = (sum + b) % n;
        }
        a = a/2;
        b = (b * 2) % n;
    }
    return sum;
}

// computes x^e  mod n without overflow
uint32_t mod_exp (uint32_t x, uint32_t e, uint32_t n){
    uint32_t ans = 1;
    uint32_t prod = x;
    for (int i = 0; i < 32; i++){
        if (1 & e ){
            ans = modmul(ans, prod, n);
        }   
        prod = modmul(prod, prod, n);
        e = e/2;
    }
    return ans;
}
// for locating primes
int is_prime(int num){
    if (num == 1){return 0;}
    int sq = sqrt((double)num);

    for (int i = 2; i <= sq; i++){
        if ( !(num % i) ){return 0;}
    }
    return 1;
}


void swap(uint32_t *a, uint32_t *b){
    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

//this is bad random number generation please god do not use this program to generate real keys
uint32_t bad_rand(){
    uint32_t r = rand() % (int)pow(2, 14);
    return r;
}

uint32_t get_prime(){
    int random_num= bad_rand();                  
    while (!is_prime(random_num)){
        random_num= bad_rand(); 
    }
    return random_num;
}



uint32_t gcd(uint32_t a, uint32_t b){
    while (b > 0){

        a = a % b;

        swap(&a, &b);

        
    }
    return a;
}



// Need to find an e such that greatest common divisor (gcd) of e and phi in 1, if so then d exists
int get_e(uint32_t phi){
    uint32_t e = bad_rand();
    while (gcd(e, phi) != 1){
        e = bad_rand();
    }
    return e;
}



uint32_t neg_mod(uint32_t neg, uint32_t m){
    while (neg % m < 0){
        neg += m;
    }
    return neg;
}


// gives d because somthing somthing extend euclidean Algorithm
uint32_t get_d(uint32_t e, uint32_t phi){
    // using pointers instead of arrays because I can
 
    uint32_t *r = calloc(100, sizeof(uint32_t));
    uint32_t *s = calloc(100, sizeof(uint32_t));
    uint32_t *t = calloc(100, sizeof(uint32_t));


    *r = e;
    *(r + 1) = phi;

    *(s) = 1;
    *(s + 1) = 0;

    *(t) = 0;
    *(t + 1) = 1;
    int i = 1;
    while( *(r + i) > 0 ){
        uint32_t q = *(r + i - 1) / *(r + i);
        *(r + i + 1) = *(r + i - 1) - q * *(r + i);
        *(s + i + 1) = *(s + i - 1) - q * *(s + i);
        *(t + i + 1) = *(t + i - 1) - q * *(t + i);
        i++;
    }
    uint32_t d;

    d = *(s + i - 1);
    if (d < 0){
        d = neg_mod(d, phi);
    }
    return d;
}


int main(){
    // just a test to show it actually works
    srand(time(0));
    uint32_t p1 = get_prime();
    uint32_t p2 = get_prime();
    uint32_t phi = (p1 - 1) * (p2 - 1);

    uint32_t e = get_e(phi); // pub key
    uint32_t d = get_d(e, phi); // privite key
    uint32_t m = p1*p2;

    uint32_t num = mod_exp(9, e, m);
    uint32_t test = mod_exp(num, d, m);
    printf("public key: %d private key: %d mod: %d\ntest: %d \n", e, d, m, test);
}
