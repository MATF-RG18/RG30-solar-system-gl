#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

static int window_width, window_height;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

const static float pi = 3.141592653589793;

static float camera_x = 10, camera_y = 0, camera_z = 10;

static int ugao = 0;

int main(int argc, char **argv)
{
    /*Inicijalizujemo GLUT*/
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /*Kreiramo prozor*/
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);

    /*Registruju se callback funkcije.*/
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

     /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(2);    

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_reshape(int width, int height){

   /* Pamte se sirina i visina prozora. */
    window_width = width;
    window_height = height; 
}

static void on_display(void){

    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesava se viewport. */
    glViewport(0, 0, window_width, window_height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60,
            window_width/(float)window_height,
            1, 50);

     /* Podesava se tacka pogleda. */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            camera_x,camera_y,camera_z,
            0, 0, 0,
            0, 0, 1
        );
 
    /*
     * Kreira se kocka i primenjuje se geometrijska transformacija na
     * istu.
     * */

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(10,0,0);
        
        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,10,0);
        
        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,10);
    glEnd();

    glPopMatrix();

    /*Crtamo 3d sferu*/
    glPushMatrix();
    glColor3f(0, 0, 1);
    glutWireSphere(3,20,20);
    glPopMatrix();

    /*Nova slika se salje na ekran*/
    glutSwapBuffers();
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        /* Zavrsava se program. */
        exit(0);
        break;
    
    case 'a':
        ugao -= 5;
        ugao %= 360;
        // printf("%d ", ugao);        
        camera_x = 10 * cos((float)ugao*pi/180);
        camera_y = 10 * sin((float)ugao*pi/180);
        //printf("%f --- %f", camera_x, camera_y);
        // printf("usao\n");
        glutPostRedisplay();
        break;

    case 'd':
        ugao += 5;
        ugao %= 360;
        // printf("%d ", ugao);        
        camera_x = 10 * cos((float)ugao*pi/180);
        camera_y = 10 * sin((float)ugao*pi/180);
        //printf("%f --- %f", camera_x, camera_y);
        // printf("usao\n");
        glutPostRedisplay();
        break;
    }
}