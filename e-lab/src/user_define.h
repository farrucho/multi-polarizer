#ifndef USER_DEFINE_H
#define USER_DEFINE_H

#define NARGUMENTS 9 
// led (0 ou 1), angulo1emsteps, angulo2emsteps, angulo3emsteps, angulo4emsteps, angulo5emsteps, polarizer a varrer(1 a 5)           , angulolimitevarrimento, PWMLed(0-255)
//                                                                                             , 0-> nao varrer | 1,2,..,5 -> varrer
#define NPROTOCOLS 1
#define BAUDRATE 115200
#define DEBUG 1 //0 for no debug (pure rec), 1 for more verbose output
#define TIMEOUT 30000 //miliseconds

#endif
