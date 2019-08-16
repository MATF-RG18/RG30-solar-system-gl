#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>

#define PI 3.1415926536
#define DOTS 99
#define NUM_OF_OBJECTS 9
#define NUM_OF_SATELITES 8
#define NUM_OF_COMETS 31

//Vreme od pocetka simulacije
static float hours;

//Parametar ubrzanja planeta
static int k;

//Indikator za stanje tajmera
static int timer_active;

//Parametri velicine, translacije i rotacije objekata
static int mercury_translation, venus_translation, earth_translation, mars_translation, sun_translation;
static int jupiter_translation, saturn_translation, uranus_translation, neptune_translation;

static float mercury_radius, venus_radius, earth_radius, mars_radius, sun_radius;
static float jupiter_radius, saturn_radius, uranus_radius, neptune_radius;

static float mercury_revolution, venus_revolution, earth_revolution, mars_revolution, sun_revolution;
static float jupiter_revolution, saturn_revolution, uranus_revolution, neptune_revolution;
static float mercury_rotation, venus_rotation, earth_rotation, mars_rotation, sun_rotation;
static float jupiter_rotation, saturn_rotation, uranus_rotation, neptune_rotation;

static int moon_radius, fobos_radius, demos_radius, io_radius, europa_radius, 
ganimed_radius, kalipso_radius, kalipsa_radius, titan_radius, atlas_radius;

static float moon_translation, fobos_translation, demos_translation, 
io_translation, europa_translation, ganimed_translation, kalipso_translation, 
kalipsa_translation, titan_translation, atlas_translation;

static float moon_revolution, fobos_revolution, demos_revolution, io_revolution, 
europa_revolution, ganimed_revolution, kalipso_revolution, kalipsa_revolution, 
titan_revolution, atlas_revolution;

static float moon_rev, fobos_rev, demos_rev, io_rev, europa_rev, 
ganimed_rev, kalipso_rev, kalipsa_rev, titan_rev, atlas_rev;

//Broj planeta
static int number_of_planets = 8;

//Indikator boja planeta i Sunca
static float r, g, b, rs, sun_r, sun_g;

//Indikator promena boja planeta i Sunca
static int color, red, green, blue, sun_red, sun_green;

//Indikatori ponovnog aktiviranja objekata
static float look_y, look_z;
static int timer, comet_on, begin_ind, ind, cyrcle;
static int on, start, end, planet_on, create_on;
static float satelite_translation;

//Deklaracije callback funkcija
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);

static void begin(void);
static void createPlanet(int i);      
static void initialize(void);
static void coordinate_system(void);
static void background(void);
static void drawCyrcle(float radius);
static void drawObjects(void);
static void drawComets(void);
static void crashObjects(float x, float y, float z, int i);
static void setPlanetColor(int i);
static void setSatelite(int i);
static void createDeleteSatelite(int i);

//Struktura - planeta
typedef struct {

    char name[11];
    float x, y, z;
    float radius;
    float translation;
    float rotation;
    float revolution;
    float teta;
    int explode;
    int has_satelite;
    int natural_satelite;
    int has_ring;
    char color;
    int draw;
    int cyrcle;
    int cyrcle_draw;
    int begin;
    int satelite_begin;
    int planet_begin;
    int planet_stop;

    //Delovi planete pri sudaru
    struct {

        float bx, by, bz;
        float translation;
        float radius;

    }part;

    //Nastanak planeta
    struct {

        float revolution;
        float translation;
        float satelite_translation;
        float end_translation;
        float sun;
        float radius;

    }start;

}Planets;

//Struktura - kometa
typedef struct {

    float x, y, z;
    float vx, vy, vz, t;
    int radius;
    int draw;

}Comets;

//Nizovi planeta i kometa
static Planets planets[NUM_OF_OBJECTS];
static Comets comets[NUM_OF_COMETS];

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(1200, 900);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Project SpaceGame");

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    //Inicijalizacija
    initialize();

    //Podesavanje boje svetla
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.7, 0.7, 0.7, 1 };

    //Koeficijenti refleksije materijala
    GLfloat ambient_coeffs[] = { 0.3, 0.7, 0.3, 1 };
    GLfloat diffuse_coeffs[] = { 0.2, 1, 0.2, 1 };
    GLfloat specular_coeffs[] = { 0.1, 0.1, 0.1, 1 };
    GLfloat shininess = 10;

    glClearColor(0.1, 0.1, 0.1, 0.1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //Omogucava se postavljanje osvetljenja
    glEnable(GL_LIGHTING);

    //Postavljaju se svojstva prvog svetla
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    //Postavljaju se svojstva materijala
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    //Ulazi se u glavnu petlju
    glutMainLoop();

    return 0;
}

//Inicijalizacija pocetnih vrednosti
static void initialize(){

    srand(time(NULL));

    //Indikatori
    hours = 0, k = 1;
    
    comet_on = 0, planet_on = 1, begin_ind = 0, ind = 1;
    timer = 0, on = 1, start = 0, end = 0, create_on = 0, cyrcle = 0;
    
    color = 1, red = 1, r = 0.5, rs = 0;
    sun_red = 1, sun_r = 0.8, sun_g = 0.1;
    timer_active = 0;

    look_y = 0, look_z = 0;

    //Parametri velicine planeta
    sun_radius = 700, mercury_radius = 200, venus_radius = 300, earth_radius = 360, mars_radius = 300,
    jupiter_radius = 500, saturn_radius = 350, uranus_radius = 300, neptune_radius = 250;

    //Parametri rastojanja planeta od Sunca
    mercury_translation = 1000, venus_translation = 1600, earth_translation = 2400; 
    mars_translation = 3200, jupiter_translation = 4200, saturn_translation = 5600;
    uranus_translation = 6500, neptune_translation = 7100, sun_translation = 0;

    //Parametri rotacije planeta oko svoje ose i oko Sunca
    sun_rotation = 360.0*k / (15 * 24), sun_revolution = 0.0,
    mercury_rotation = 360.0*k / (59 * 24), mercury_revolution = 360.0 * 2*k*hours / (88 * 24),
    venus_rotation = 360.0*k / (243 * 24), venus_revolution = 360.0 * 2*k*hours / (225 * 24),
    earth_revolution = 360.0*k*hours * 2 / (365 * 24), earth_rotation = 360.0*k / (1 * 24),
    mars_rotation = 360.0*k / (1 * 24), mars_revolution = 360.0 * 2*k*hours / (687 * 24),
    jupiter_rotation = 3600.0*k / (10 * 24), jupiter_revolution = 3600.0 * 2*k*hours/ (12 * 365 * 24),
    saturn_rotation = 3600.0*k / (11 * 24), saturn_revolution = 3600.0 * 4*k*hours / (30 * 365 * 24),
    uranus_rotation = 3600.0*k / (18 * 24), uranus_revolution = 3600.0 * 9*k*hours / (84 * 365 * 24),
    neptune_rotation = 3600.0*k / (20 * 24), neptune_revolution = 3600.0 * 15*k*hours / (165 * 365 * 24);

    //Nizovi parametara planeta
    char names[NUM_OF_OBJECTS][11] = {  "SUNCE", "MERKUR", "VENERA", "ZEMLJA", "MARS", "JUPITER",
                                        "SATURN", "URAN", "NEPTUN"};

    float translations[NUM_OF_OBJECTS] = { sun_translation, mercury_translation, venus_translation, 
                                            earth_translation, mars_translation, jupiter_translation, 
                                            saturn_translation, uranus_translation, neptune_translation
                                         };
    
    float radius[NUM_OF_OBJECTS] = { sun_radius, mercury_radius, venus_radius, earth_radius, mars_radius,
                                     jupiter_radius, saturn_radius, uranus_radius, neptune_radius
                                   };
    
    float rotations[NUM_OF_OBJECTS] = { sun_rotation, mercury_rotation, venus_rotation, earth_rotation, mars_rotation,
                                        jupiter_rotation, saturn_rotation, uranus_rotation, neptune_rotation
                                      };
    
    float revolutions[NUM_OF_OBJECTS] = {  sun_revolution, mercury_revolution, venus_revolution, 
                                           earth_revolution, mars_revolution, jupiter_revolution, 
                                           saturn_revolution, uranus_revolution, neptune_revolution
                                        };

    float start_revolutions[NUM_OF_OBJECTS] = { 100, 200, 300, 400, 500, 600, 700, 800, 3900 };
    float satelite_translation[NUM_OF_OBJECTS] = { 0, 130, 220, 270, 220, 380, 250, 180, 180 };
    int cyrcles[NUM_OF_OBJECTS] = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000 };

    //Nizovi parametara satelita
    int has_satelite[NUM_OF_OBJECTS] = { 0, 0, 0, 1, 2, 4, 3, 0, 0 };
    int natural_satelite[NUM_OF_OBJECTS] = { 0, 0, 0, 1, 2, 4, 3, 0, 0 };

    //Inicijalizacija parametara planeta
    int i, j;

    for(i=0; i<NUM_OF_OBJECTS; i++){

        strcpy(planets[i].name, names[i]);

        planets[i].x = translations[i];
        planets[i].y = 0;
        planets[i].z = 0;
        planets[i].radius = radius[i];
        planets[i].translation = translations[i];
        planets[i].rotation = rotations[i];
        planets[i].revolution = start_revolutions[i];
        planets[i].teta = 0;
        planets[i].explode = 0;
        planets[i].has_satelite = has_satelite[i];
        planets[i].natural_satelite = natural_satelite[i];
        planets[i].has_ring = 0;
        planets[i].color = 0;

        planets[i].draw = 0;
        planets[i].cyrcle = 1;
        planets[i].cyrcle_draw = cyrcles[i];
        planets[i].begin = 1;

        planets[i].planet_begin = 1;
        planets[i].satelite_begin = 1;
        planets[i].planet_stop = 0;

        //Inicijalizacija planeta pri nastanku
        planets[i].start.revolution = start_revolutions[i];
        planets[i].start.translation = translations[i];
        planets[i].start.satelite_translation = 10000;
        planets[i].start.end_translation = satelite_translation[i];
        planets[i].start.sun = 150;
        planets[i].start.radius = 10;

        //Inicijalizija delova planeta pri sudaru
        planets[i].part.bx = 0;
        planets[i].part.by = 0;
        planets[i].part.bz = 0;
        planets[i].part.radius = 10;
        planets[i].part.translation = 0;
    }

    //Inicijalizacija parametara kometa
    for(i=0; i<NUM_OF_COMETS; i++){
        
        comets[i].x = -7000 + rand()%14000;
        comets[i].y = -7000 + rand()%14000;
        
        if((rand()%100)%2 == 0)
            comets[i].z = 12000;
        else  
            comets[i].z = -12000;

        //Bira se nasumicna tacka u xy ravni
        float spot_x = -7000 + rand()%14000;
        float spot_y = -7000 + rand()%14000;
        float spot_z = 0;

        //Odredjuje se vektor pravca komete
        comets[i].vx = comets[i].x - spot_x;
        comets[i].vy = comets[i].y - spot_y;
        comets[i].vz = comets[i].z - spot_z;
        comets[i].t = 0.02;

        comets[i].radius = 0;
        comets[i].draw = 0;
    }
}

//Nastanak planeta
static void begin(){

    int i = 0;

    //Pocetna eksplozija
    glPushMatrix();
        glColor3f(rs, 0, 0);
        glutSolidSphere(planets[i].start.sun, 125, 125);             
    glPopMatrix();

    for(i=0; i<NUM_OF_OBJECTS; i++){

        glPushMatrix();
    
            glRotatef(planets[i].start.revolution, 0, 0, 1);
            glTranslatef(planets[i].start.translation, 0, 0);
                
            glPushMatrix();

                //Iscrtavamo planete
                if(planets[i].planet_begin){
                    
                    glColor3f(1, 0, 0);
                    glutSolidSphere(planets[i].start.radius, 125, 125);
                }

            glPopMatrix(); 

            //Iscrtavamo satelite
            glPushMatrix();
                if(planets[i].satelite_begin && create_on){

                    //Simulacija kreiranja planeta 
                    createPlanet(i);       
                }
            glPopMatrix();

        glPopMatrix();  
    }
}

//Kreiranje planeta
static void createPlanet(int i){

    int j;

    float satelite_radius[NUM_OF_OBJECTS]= { 0, 60, 70, 80, 70, 90, 70, 70, 70 };

    float satelite_revolution[NUM_OF_OBJECTS] = { 0, 27*5*hours, 30*5*hours, 33*5*hours, 360*hours, 
                                                  360*7*hours, 360*7*hours, 360*7*hours, 360*7*hours
                                                };
    float mercury_satelite_rev[NUM_OF_SATELITES] = { 32*10, 32*12, 32*14, 32*16, 32*18, 
                                                    32*18, 32*20, 32*22
                                                   };
    float venus_satelite_rev[NUM_OF_SATELITES] = { 30*6, 30*8, 30*10, 30*12, 30*14, 
                                                  30*16, 30*18, 30*20
                                                 };
    float earth_satelite_rev[NUM_OF_SATELITES] = { 28*8, 28*10, 28*12, 28*14, 28*16, 
                                                  28*18, 28*20, 28*22
                                                 };
    float mars_satelite_rev[NUM_OF_SATELITES] = { 30*24, 30*22, 30*20, 30*18, 
                                                 30*16, 30*14, 30*12, 30*10
                                                };
    float jupiter_satelite_rev[NUM_OF_SATELITES] = { 100*24*2, 130*24*2, 160*24*2, 190*24*2, 
                                                    220*24, 250*24, 280*24, 310*24
                                                   };
    float saturn_satelite_rev[NUM_OF_SATELITES] = { 110*24, 140*24, 170*24, 200*24, 
                                                    230*24, 260*24, 290*24, 320*24
                                                  };
    float uranus_satelite_rev[NUM_OF_SATELITES] = { 130*24, 150*24, 190*24, 210*24, 
                                                    240*24, 280*24, 310*24, 370*24
                                                  };                                             
    float neptune_satelite_rev[NUM_OF_SATELITES] = { 120*24, 140*24, 180*24, 200*24, 
                                                    230*24, 270*24, 300*24, 350*24
                                                   };
    float angle = rand()%180;
    int satelites = 10;

    //Dodavanje satelita planetama
    switch(i){
        case 0: //Sunce nema satelite 
                break;
        //Merkur        
        case 1: {

            for(j=0; j < satelites; j++){

                glRotatef(angle+=5, 0, 1, 0);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / mercury_satelite_rev[j], 0, 0, 1);
                    glTranslatef(planets[i].start.satelite_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Venera
        case 2: {

            for(j=0; j < satelites; j++){

                glRotatef(angle+=5, 0, 1, 0);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / venus_satelite_rev[j], 0, 0, 1);
                    glTranslatef(planets[i].start.satelite_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }  
                break;
        //Zemlja
        case 3: {

            for(j=0; j < satelites; j++){

                glRotatef(angle+=5, 0, 1, 0);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / earth_satelite_rev[j], 0, 0, 1);
                    glTranslatef(planets[i].start.satelite_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Mars
        case 4: {

            for(j=0; j < satelites; j++){

                glRotatef(angle+=5, 0, 1, 0);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / mars_satelite_rev[j], 0, 0, 1);
                    glTranslatef(planets[i].start.satelite_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Jupiter
        case 5: {

            for(j=0; j < satelites; j++){

                glRotatef(angle+=5, 0, 1, 0);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / jupiter_satelite_rev[j], 0, 0, 1);
                    glTranslatef(planets[i].start.satelite_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Saturn
        case 6: {

            //Generisu se Saturnovi prsteni
             if(planets[i].has_ring){
                
                glPushMatrix();
                    glRotatef(-40, 0, 1, 0);    
                    glColor3f(0.5, 0.7, 1);
                    GLUquadric* disk1 = gluNewQuadric();
                    gluDisk(disk1, 450, 470, 125, 125);
                    glColor3f(0.9, 0.4, 0.1);
                    GLUquadric* disk2 = gluNewQuadric();
                    gluDisk(disk2, 480, 510, 125, 125);
                    glColor3f(0.5, 0.7, 0.9);
                    GLUquadric* disk3 = gluNewQuadric();
                    gluDisk(disk3, 550, 700, 125, 125);
                glPopMatrix();
            }

            for(j=0; j < satelites; j++){

                glRotatef(angle+=5, 0, 1, 0);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / saturn_satelite_rev[j], 0, 0, 1);
                    glTranslatef(planets[i].start.satelite_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Uran
        case 7: {

            for(j=0; j < satelites; j++){

                glRotatef(angle+=5, 0, 1, 0);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / uranus_satelite_rev[j], 0, 0, 1);
                    glTranslatef(planets[i].start.satelite_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Neptun
        case 8: {

            for(j=0; j < satelites; j++){

                glRotatef(angle+=5, 0, 1, 0);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / neptune_satelite_rev[j], 0, 0, 1);
                    glTranslatef(planets[i].start.satelite_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        } 
                break;
        default: 
                break;
    }
} 

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    
    //Kraj programa
    case 27:
        exit(0);
        break;

    //Pocetak simulacije
    case 'g':
    case 'G':
        if (!timer_active) {
            glutTimerFunc(50, on_timer, 0);
            timer_active = 1;

            start = 1;
        }
        glEnable(GL_LIGHTING);
        break;

    //Postavljanje boja planetama
    case 's':
    case 'S':
        glDisable(GL_LIGHTING);
        break;

    //Ubrzava se rotacija planeta
    case '+':
        k += 1;
        if(k >= 11)
            k = 1;
        break;

    //Usporava se rotacija planeta
    case '-':
        k -= 1;
        if(k <= 2)
            k = 1;
        break;

    //Pauzira se simulacija
    case 'p':
    case 'P':
        timer_active = 0;
        break;
    }
}

static void on_timer(int value)
{
    //Provera da li callback dolazi od odgovarajuceg tajmera
    if (value != 0)
        return;

    //Azurira se trajanje simulacije
    hours += 31;

    //Isijavanje Sunca
    if(sun_red == 1)
    {
        sun_g += 0.01;
        sun_r += 0.025;
        if(sun_r > 1)
        {
            sun_r = 0.7;
            sun_g = 0.3;
            sun_red = 0;
            sun_green = 1;
        }
    }
    if(sun_green == 1)
    {
        sun_g += 0.01;
        sun_r += 0.025;
        if(sun_r > 1)
        {
            sun_r = 0.7;
            sun_g = 0.3;
            sun_green = 0;
            sun_red = 1;
        }
    }

    //Promena boje planetama 
    if(red == 1)
    {
        g = 0.0;
        b = 0.0;

        //Ako je ostala jedna planeta
        //duze ostaje crvena da bi lakse
        //bila pogodjena kometom
        if(number_of_planets == 1)
            r += 0.005;
        else
            r += 0.008;
        
        if(r > 1)
        {
            r = 0.9;
            g = 0.5;
            red = 0;
            green = 1;
        }
    }

    if(green == 1)
    {
        r = 0.0;
        b = 0.0;
        g += 0.009;

        if(g > 1)
        {
            g = 0.9;
            b = 0.5;
            green = 0;
            blue = 1;
        }
    }

    if(blue == 1)
    {
        g = 0.0;
        r = 0.0;
        b += 0.009;

        if(b > 1)
        {
            b = 0.9;
            r = 0.5;
            blue = 0;
            red = 1;
        }
    }

    int i, c;

    //Animira se nastanak planeta
    for(i=0; i<NUM_OF_OBJECTS; i++){

        //Zumiranje planeta
        look_y += 15;
        look_z += 10;
        if(look_y  > 14000)
            look_y = 14000;
         if(look_z  > 18000)
            look_z = 18000;
        
        //Prva eksplozija
        if(i == 0 && create_on == 0){

            planets[i].start.sun += 105;

            if(planets[i].start.sun > 10000){
                
                planets[i].start.sun = 700;
                rs = 1;
                create_on = 1;  
            }
        }

        //Delovi eksplozije se grupisu oko svake planete
        if(!planets[i].planet_stop)
            planets[i].start.satelite_translation -= 50;

        if(planets[i].start.satelite_translation < 300){

            planets[i].start.satelite_translation -= 1;

            if(planets[i].start.satelite_translation < planets[i].start.end_translation){

                planets[i].start.satelite_translation =  planets[i].start.end_translation;
                planets[i].planet_stop = 1; 
                planets[i].has_ring = 1;
            }
        }

        //Formiraju se planete
        if(planets[i].planet_stop){

            cyrcle += 15;

            if(i == 0)
                planets[i].start.radius += 10;
            else if(i == 5)
                planets[i].start.radius += 5;
            else
            {
                planets[i].start.radius += 4;
            }  
        }

        begin_ind += 5;

        if(planets[i].start.radius > planets[i].radius){
                    
            planets[i].start.radius = planets[i].radius;
            planets[i].planet_stop = 0;
        }

        if(begin_ind > 13400 && ind == 1){

            for(c=0; c<NUM_OF_OBJECTS; c++){
                
                planets[c].planet_begin = 0;
                planets[c].satelite_begin = 0;
                planets[c].has_ring = 0;
                planets[c].start.sun = 0;

                planets[c].draw = 1;
            }

            //Ukljucuju se komete
            for(c=0; c<NUM_OF_COMETS; c++){

                comets[c].draw = 1;
                comets[c].radius = 50;
            }

            begin_ind = 12400;
            ind = 0;
            cyrcle = 10000;
        }
    }

    //Animiraju se komete
    float t;
    float spot_x, spot_y, spot_z;

    for(i=0; i<NUM_OF_COMETS; i++){

        comets[i].x -= comets[i].vx * comets[i].t;
        comets[i].y -= comets[i].vy * comets[i].t;
        comets[i].z -= comets[i].vz * comets[i].t;

        //Parametri nove komete
        if(comets[i].x < -16000 || comets[i].y < -16000 || comets[i].z < -16000
            || comets[i].x > 16000 || comets[i].y > 16000 || comets[i].z > 16000){
                
            comets[i].x = -8000 + rand()%16000;
            comets[i].y = -8000 + rand()%16000;
            if((rand()%100)%2 == 0)
                comets[i].z = 14000;
            else
                comets[i].z = -14000;
                
            t = (float)(rand()%100) <= 5 ? 0.01 : 0.02;
            comets[i].t = t;
           
            //Bira se nasumicna tacka u ravni
            spot_x = -7000 + rand()%14000;
            spot_y = -7000 + rand()%14000;
            spot_z = 0;

            //Odredjuje se vektor pravca komete
            comets[i].vx = comets[i].x - spot_x;
            comets[i].vy = comets[i].y - spot_y;
            comets[i].vz = comets[i].z - spot_z;

            //Ukljucuje se ponovo kometa koja je
            //prethodno pogodila planetu
            comets[comet_on].draw = 1;
        }
        
        //Realizuje se pogodak
        crashObjects(comets[i].x, comets[i].y, comets[i].z, i);
    }

    //Animira se sudar sa kometom
    int j, l;

    for(j=0; j<NUM_OF_OBJECTS; j++){

        //Ako je doslo do sudara
        if(planets[j].explode){
                
            //Ako je objekat - planeta
            if(j){

                planets[j].part.radius += 15;

                planets[j].part.bx += rand()%3;
                planets[j].part.by += rand()%3;
                planets[j].part.bz = 0;

                if( planets[j].part.bx > 35 || planets[j].part.bx > 35){

                    planets[j].part.radius = 10;

                    planets[j].explode = 0;
                }
            }
            //Ako je objekat - Sunce
            else if(j == 0 && end == 0)
            {
                planets[j].radius += 35;

                if(planets[j].radius > 5000){

                    planets[j].radius = 150;

                    for(l=0; l<NUM_OF_COMETS; l++)
                        comets[l].draw = 0;

                    on = 0;
                    end = 1;

                    printf("Unisteno je: SUNCE!\n");

                    start = 0;
                }
            }       
        }
    }

    glutPostRedisplay();

    //Po potrebi se ponovo postavlja tajmer
    if (timer_active)
        glutTimerFunc(50, on_timer, 0);
}

//Kolizija objekata
static void crashObjects(float cx, float cy, float cz, int cj){

    int i, j, l;
    float distance, px, py, pz;
    comet_on = cj;

    for(i=0; i<NUM_OF_OBJECTS; i++){

        //Koordinate planeta
        px = planets[i].x, py = planets[i].y, pz = planets[i].z;
        
        //Rastojanje izmedju planeta i komete
        distance = sqrt(pow(px - cx, 2) + pow(py - cy, 2) + pow(pz - cz, 2));

        //Ako je doslo do sudara planete i komete
        if(distance < (float)(planets[i].radius + comets[comet_on].radius)){

            //Kometa se unistava
            comets[comet_on].draw = 0;

            //i != 0 stitimo Sunce
		
            //Ako je boja planete - plava
            if(i != 0 && planets[i].color == 'b'){
               
                //Kometa postaje njen satelit - ne dozvoljava se vise od 9 satelita po planeti
                planets[i].has_satelite >= 9 ? planets[i].has_satelite = 9 : planets[i].has_satelite++;
                
                printf("PLAVA - Kreiran je satelit: %d na planeti: %s\n", 
                        planets[i].has_satelite, planets[i].name);
            }

            //Ako je boja planete - zelena
            if(i != 0 && planets[i].color == 'g'){
            
                //Ako planeta ima bar jedan satelit
                //kometa ga unistava
                //planeta ostaje
                if(planets[i].has_satelite > 0){
                    
                    planets[i].has_satelite--;

                    printf("ZELENA - Unisten je satelit na planeti: %s, ostalo satelita: %d\n", 
                            planets[i].name, planets[i].has_satelite);
                }

                //Ako planeta nema satelite biva unistena
                if(planets[i].has_satelite == 0){
                    
                    planets[i].draw = 0;
                    planets[i].cyrcle = 0;
                    planets[i].explode = 1;
                    planets[i].radius = -comets[comet_on].radius;

                    number_of_planets--; 

                    printf("ZELENA - Unistena je planeta: %s, ostalo satelita: %d\n", 
                            planets[i].name, planets[i].has_satelite);
                    printf("Trenutni broj planeta je: %d ( ", number_of_planets);
                    for(l=1; l<NUM_OF_OBJECTS; l++){
                        
                        if(planets[l].draw)
                            printf("%s - ", planets[l].name);
                    }
                    printf(")\n");
                }
            }

            //Ako je boja planete - crvena
            //automatski biva unistena
            //bez obzira na broj satelita
            if(i != 0 && planets[i].color == 'r'){
              
                planets[i].draw = 0;
                planets[i].cyrcle = 0;
                planets[i].explode = 1;
                planets[i].radius = -comets[comet_on].radius;

                number_of_planets--;

                printf("CRVENA - Unistena je planeta: %s, ostalo satelita: %d\n", 
                        planets[i].name, planets[i].has_satelite);
                printf("Trenutni broj planeta je: %d ( ", number_of_planets);
                for(l=1; l<NUM_OF_OBJECTS; l++){
                    
                    if(planets[l].draw)
                        printf("%s - ", planets[l].name);
                }
                printf(")\n");
            }

            //Ako su unistene sve planete 
            //eksplodira i Sunce pri sledecem pogotku
            if(i == 0 && number_of_planets == 0)
                planets[i].explode = 1;
        }
    }
}

//Podesava se pozadina
static void background(void){

    float x = -15000 + rand()%30000;
    float y = -15000 + rand()%30000;
    float z = -15000 + rand()%30000;

    glPushMatrix();
        glTranslatef(x, y, z);
        glColor3f(1, 1, 1);
        glutSolidSphere(50, 125, 125);
    glPopMatrix();
}

//Iscrtavaju se objekti - sunce, planete, sateliti
static void drawObjects(){

    int i, j;

    //Parametri animacije objekata
    float rev1[NUM_OF_OBJECTS] = {0, 360.0*2, 360.0*2, 360.0*2, 360.0*2, 3600.0*2, 
                                  3600.0*4, 3600.0*9, 3600.0*15};
    float rev2[NUM_OF_OBJECTS] = {1, 88*24, 225*24, 365*24, 684*24, 12*365*24, 
                                  30*365*24, 84*365*24, 165*365*24};
    
    float rot1[NUM_OF_OBJECTS] = {360.0, 360.0, 360.0, 360.0, 360.0, 3600.0, 3600.0, 3600.0, 3600.0};
    float rot2[NUM_OF_OBJECTS] = {15*24, 59*24, 243*24, 1*24, 1*24, 10*24, 11*24, 18*24, 20*24};

    //Generisu se planete
    for(i=0; i<NUM_OF_OBJECTS; i++){

        glPushMatrix();

            planets[i].rotation = rot1[i]*hours / rot2[i];
            planets[i].revolution = rev1[i]*k*hours / rev2[i];
 
            if(on && create_on && planets[i].cyrcle){

                if(planets[i].cyrcle_draw < cyrcle)
                    drawCyrcle(planets[i].translation);
            }

            glRotatef(planets[i].revolution, 0, 0, 1);
            glTranslatef(planets[i].translation, 0, 0);
                
            glPushMatrix();

                //Iscrtavamo planete
                if(planets[i].draw){

                    //Podesava se boja planete
                    setPlanetColor(i);

                    glRotatef(planets[i].rotation, 0, 0, 1);
                    glutSolidSphere(planets[i].radius, 125, 125);

                    //Odredjuju se koordinate planeta
                    planets[i].teta = ((int)planets[i].revolution)%360;
                    planets[i].x = planets[i].translation * cos(planets[i].teta * PI/180);
                    planets[i].y = planets[i].translation * sin(planets[i].teta * PI/180);
                    planets[i].z = 0;               
                }

                //Ako je doslo do sudara
                if(planets[i].explode){
                            
                    //Iscrtavamo bljesak
                    glPushMatrix();
                        glColor3f(1, 1, 1);
                        glutSolidSphere(planets[i].part.radius, 125, 125);
                    glPopMatrix();
                }

            glPopMatrix(); 

            //Iscrtavamo satelite
            glPushMatrix();
                if(planets[i].draw){
                    
                    //Iscrtavaju se prirodni sateliti planeta
                    setSatelite(i);

                    //Kreiraju se - ili brisu - "vestacki" sateliti
                    createDeleteSatelite(i);           
                }
            glPopMatrix();

        glPopMatrix();  
    }
}

//Postavljaju se boje planetama
static void setPlanetColor(int c){

    //Parametari koji odredjuju boju planeta
    //r - g - b staticke promenljive

    if(number_of_planets == 1 && c != 0)
        glColor3f(r, g, b);

    else if(c == 0)
        glColor3f(sun_r, sun_g, 0);

    else    
    {
        switch(c){
            case 1: glColor3f(r, g, b);
                    break;
            case 2: glColor3f(g, b, r);
                    break;
            case 3: glColor3f(b, r, g);
                    break;
            case 4: glColor3f(r, g, b);
                    break;
            case 5: glColor3f(g, b, r);
                    break;
            case 6: glColor3f(b, r, g);
                    break;
            case 7: glColor3f(r, g, b);
                    break;
            case 8: glColor3f(g, b, r);
                    break;
            default:
                    break;
        }
    }

    //Dodeljujemo planeti boju
    if(g == 0 && b == 0)
        planets[c].color = 'r';
    else if(r == 0 && b == 0)
        planets[c].color = 'g';
    else if(r == 0 && g == 0)
        planets[c].color = 'b';
}

//Postavljaju se prirodni sateliti planeta
static void setSatelite(int i){

    float moon_revolution = 36*5*hours, fobos_revolution = 360*hours, 
            demos_revolution = 360*hours, io_revolution = 360*5*hours, 
            europa_revolution = 360*5*hours, ganimed_revolution = 360*5*hours, 
            kalipso_revolution = 360*5*hours, kalipsa_revolution = 360*5*hours, 
            titan_revolution = 360*5*hours, atlas_revolution = 360*5*hours;

    float moon_rev = 28*24, fobos_rev = 11*3*24, demos_rev = 30*3*24, 
            io_rev = 100*2*24, europa_rev = 200*2*24, ganimed_rev = 300*2*24, 
            kalipso_rev = 400*2*24, kalipsa_rev = 100*2*24, 
            titan_rev = 200*2*24, atlas_rev = 300*2*24;

    float moon_translation = 470, fobos_translation = 400, demos_translation = 410, 
            io_translation = 620, europa_translation = 620, ganimed_translation = 630, 
            kalipso_translation = 640, kalipsa_translation = 470, 
            titan_translation = 460, atlas_translation = 480;

    float moon_radius = 100, fobos_radius = 90, demos_radius = 80, io_radius = 90, 
            europa_radius = 80, ganimed_radius = 150, kalipso_radius = 100, 
            kalipsa_radius = 100, titan_radius = 80, atlas_radius = 90;

    switch(i){
        case 0: //Sunce nestaje kada su unistene sve planete 
                break;
        case 1: //Merkur nema prirodnih satelita 
                break;
        case 2: //Venera nema prirodnih satelita
                break;
        //Zemlja
        case 3: {

            if(planets[3].has_satelite >= 1){
                
                glPushMatrix();
                    glRotatef(moon_revolution/moon_rev, 0, 0, 1);
                    glTranslatef(moon_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(moon_radius, 125, 125);
                glPopMatrix();
            }
        }
                break;
        //Mars
        case 4: {

            if(planets[4].has_satelite >= 2){
                
                glPushMatrix();
                    glRotatef(fobos_revolution/fobos_rev, 0, 0, 1);
                    glTranslatef(fobos_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(fobos_radius, 125, 125);
                glPopMatrix();
            }

            if(planets[3].has_satelite >= 1){
                
                glPushMatrix();
                    glRotatef(demos_revolution/demos_rev, 0, 0, 1);
                    glTranslatef(demos_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(demos_radius, 125, 125);
                glPopMatrix();
            }
        }
                break;
        //Jupiter
        case 5: {
            
            if(planets[5].has_satelite >= 4){
                
                glPushMatrix();
                    glRotatef(io_revolution/io_rev, 0, 0, 1);
                    glTranslatef(io_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(io_radius, 125, 125);
                glPopMatrix();
            }

            if(planets[5].has_satelite >= 3){
                
                glPushMatrix();
                    glRotatef(europa_revolution/europa_rev, 0, 0, 1);
                    glTranslatef(europa_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(europa_radius, 125, 125);
                glPopMatrix();
            }

            if(planets[5].has_satelite >= 2){
                
                glPushMatrix();
                    glRotatef(ganimed_revolution/ganimed_rev, 0, 0, 1);
                    glTranslatef(ganimed_translation, 0, 0);  
                    glColor3f(1, 1, 1);
                    glutSolidSphere(ganimed_radius, 125, 125);
                glPopMatrix();
            }

            if(planets[5].has_satelite >= 1){
                
                glPushMatrix();
                    glRotatef(kalipso_revolution/kalipso_rev, 0, 0, 1);
                    glTranslatef(kalipso_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(kalipso_radius, 125, 125);
                glPopMatrix();
            }
        }
                break;
        //Saturn
        case 6: {
            
            //Generisu se Saturnovi prsteni
            glPushMatrix();
                glRotatef(-40, 0, 1, 0);    
                glColor3f(0.5, 0.7, 1);
                GLUquadric* disk1 = gluNewQuadric();
                gluDisk(disk1, 450, 470, 125, 125);
                glColor3f(0.9, 0.4, 0.1);
                GLUquadric* disk2 = gluNewQuadric();
                gluDisk(disk2, 480, 510, 125, 125);
                glColor3f(0.5, 0.7, 0.9);
                GLUquadric* disk3 = gluNewQuadric();
                gluDisk(disk3, 550, 700, 125, 125);
            glPopMatrix();

            glRotatef(-40, 0, 1, 0);
            if(planets[6].has_satelite >= 3){
                
                glPushMatrix();
                    glRotatef(kalipsa_revolution/kalipsa_rev, 0, 0, 1);
                    glTranslatef(kalipsa_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(kalipsa_radius, 125, 125);
                glPopMatrix();
            }

            if(planets[6].has_satelite >= 2){
                
                glPushMatrix();
                    glRotatef(titan_revolution/titan_rev, 0, 0, 1);
                    glTranslatef(titan_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(titan_radius, 125, 125);
                glPopMatrix();
            }

            if(planets[6].has_satelite >= 1){
                
                glPushMatrix();
                    glRotatef(atlas_revolution/atlas_rev, 0, 0, 1);
                    glTranslatef(atlas_translation, 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(atlas_radius, 125, 125);
                glPopMatrix();
            }
        }
                break;
        case 7: //Uran nema prirodnih satelita
                break;
        case 8: //Neptun nema prirodnih satelita
                break;
        default:
                break;
    }
}

//Kreiraju se "vestacki" sateliti
static void createDeleteSatelite(int i){

    int j;
    float satelite_translation[NUM_OF_OBJECTS] = { 0, 280, 370, 550, 450, 750, 650, 400, 300};

    float satelite_radius[NUM_OF_OBJECTS]= { 0, 70, 80, 90, 80, 100, 90, 80, 80 };

    float satelite_revolution[NUM_OF_OBJECTS] = { 0, 27*5*hours, 30*5*hours, 33*5*hours, 360*hours, 
                                                   360*7*hours, 360*7*hours, 360*7*hours, 360*7*hours
                                                 };
    float mercury_satelite_rev[NUM_OF_SATELITES] = { 32*10, 32*12, 32*14, 32*16, 32*18, 
                                                    32*18, 32*20, 32*22
                                                  };
    float venus_satelite_rev[NUM_OF_SATELITES] = { 30*6, 30*8, 30*10, 30*12, 30*14, 
                                                  30*16, 30*18, 30*20
                                                };
    float earth_satelite_rev[NUM_OF_SATELITES] = { 28*8, 28*10, 28*12, 28*14, 28*16, 
                                                  28*18, 28*20, 28*22
                                                };
    float mars_satelite_rev[NUM_OF_SATELITES] = { 30*24, 30*22, 30*20, 30*18, 
                                                 30*16, 30*14, 30*12, 30*10
                                               };
    float jupiter_satelite_rev[NUM_OF_SATELITES] = { 100*24*2, 130*24*2, 160*24*2, 190*24*2, 
                                                    220*24, 250*24, 280*24, 310*24
                                                  };
    float saturn_satelite_rev[NUM_OF_SATELITES] = { 110*24, 140*24, 170*24, 200*24, 
                                                    230*24, 260*24, 290*24, 320*24
                                                  };
    float uranus_satelite_rev[NUM_OF_SATELITES] = { 130*24, 150*24, 190*24, 210*24, 
                                                    240*24, 280*24, 310*24, 370*24
                                                 };                                             
    float neptune_satelite_rev[NUM_OF_SATELITES] = { 120*24, 140*24, 180*24, 200*24, 
                                                    230*24, 270*24, 300*24, 350*24
                                                  };
    float angle = -40;
    int satelites = planets[i].has_satelite - planets[i].natural_satelite;

    switch(i){
        case 0: //Sunce nema satelite 
                break;
        //Merkur
        case 1: {

            for(j=0; j < satelites; j++){

                glRotatef(angle, 0, 0, 1);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / mercury_satelite_rev[j], 0, 0, 1);
                    glTranslatef(satelite_translation[i], 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Venera
        case 2: {

            for(j=0; j < satelites; j++){

                glRotatef(angle, 0, 0, 1);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / venus_satelite_rev[j], 0, 0, 1);
                    glTranslatef(satelite_translation[i], 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }  
                break;
        //Zemlja
        case 3: {

            for(j=0; j < satelites; j++){

                glRotatef(angle, 0, 0, 1);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / earth_satelite_rev[j], 0, 0, 1);
                    glTranslatef(satelite_translation[i], 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Mars
        case 4: {

            for(j=0; j < satelites; j++){

                glRotatef(angle, 0, 0, 1);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / mars_satelite_rev[j], 0, 0, 1);
                    glTranslatef(satelite_translation[i], 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Jupiter
        case 5: {

            for(j=0; j < satelites; j++){

                glRotatef(angle, 0, 0, 1);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / jupiter_satelite_rev[j], 0, 0, 1);
                    glTranslatef(satelite_translation[i], 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Saturn
        case 6: {

            for(j=0; j < satelites; j++){

                glRotatef(angle, 0, 0, 1);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / saturn_satelite_rev[j], 0, 0, 1);
                    glTranslatef(satelite_translation[i], 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Uran
        case 7: {

            for(j=0; j < satelites; j++){

                glRotatef(angle, 0, 0, 1);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / uranus_satelite_rev[j], 0, 0, 1);
                    glTranslatef(satelite_translation[i], 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        }
                break;
        //Neptun
        case 8: {

            for(j=0; j < satelites; j++){

                glRotatef(angle, 0, 0, 1);
                glPushMatrix();
                    glRotatef(satelite_revolution[i] / neptune_satelite_rev[j], 0, 0, 1);
                    glTranslatef(satelite_translation[i-1], 0, 0);
                    glColor3f(1, 1, 1);
                    glutSolidSphere(satelite_radius[i], 125, 125);
                glPopMatrix();
            }    
        } 
                break;
        default: 
                break;
    }
}

//Generisu se komete
static void drawComets(){

    int i;
    float comet_angle;

    for(i=0; i<NUM_OF_COMETS; i++) {

        if(comets[i].draw){

			glPushMatrix();
				
                glTranslatef(comets[i].x, comets[i].y, comets[i].z);
                glColor3f(sun_r, sun_g, 0);
                glutSolidSphere(comets[i].radius, 125, 125);   
                
			glPopMatrix();
		}
    }
}

//Iscrtava se putanja planeta
static void drawCyrcle(float radius){
    
    int i;
    glBegin(GL_LINE_STRIP);

        glLineWidth(7);

            for (i = 0; i < DOTS; i++) {
                
                //Dok se kreiraju planete
                if(begin_ind < 13500){
                    
                    glColor3f(1*sin(i), 0, 0);
                    glVertex2f(
                                cos(2 * i * PI / DOTS) * radius,
                                sin(2 * i * PI / DOTS) * radius
                              );
                }
                //Nakon kreiranja planeta
                else
                {
                    glColor3f(r*sin(i), g*tan(i), b*cos(i));
                    glVertex2f(
                                cos(2 * i * PI / DOTS) * radius,
                                sin(2 * i * PI / DOTS) * radius
                              );
                }
            }
    glEnd();
}

//Postavlja se koordinatni sistem
static void coordinate_system(void){

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(-10000,0,0);
        glVertex3f(10000,0,0);
        
        glColor3f(0,1,0);
        glVertex3f(0,-10000,0);
        glVertex3f(0,10000,0);
        
        glColor3f(0,0,1);
        glVertex3f(0,0,-10000);
        glVertex3f(0,0,10000);
    glEnd();
}

static void on_reshape(int width, int height)
{
    //Postavlja se viewport
    glViewport(0, 0, width, height);

    //Postavljaju se parametri projekcije
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 50000);
}

static void on_display(void)
{
    GLint i;
    float comet_angle;

    //Postavlja se pozicija svetla
    GLfloat light_position[] = { 1000, -1000, 4500, 0 };

    //Postavlja se boja svih piksela na zadatu boju pozadine
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Postavlja se model sencanja
    glShadeModel(GL_FLAT);

    //Postavlja se vidna tacka
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Podesava se pozicija svetla
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    //Podesava se pozicija kamere
    gluLookAt(1500, -25000+look_y, 25500-look_z, 0, 0, 0, 0, 1, 0);

    if(start){
        
        begin();

        if(on){

            //Iscrtava se koordinatni pocetak
            //coordinate_system();

            //Podesava se pozadina
            background();
    
            //Generisu se komete
            drawComets();
        }
    }

    //Generisu se objekti
    drawObjects();

    //Postavlja se nova slika u prozor
    glutSwapBuffers();
}
