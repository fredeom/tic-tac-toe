#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <linmath.h>

static double xpos = 0, ypos = 0;

static int width, height, turn = 1;

static int do_redraw = 1;

static int field [] = {
  -1, -1, -1,
  -1, -1, -1,
  -1, -1, -1
};

float cross_vertices [] = {
  -0.45, 0.45, 0,
  0.45, -0.45, 0,
  0.45, 0.45, 0,
  -0.45, -0.45, 0
};

static void checkWinner(void) {
  fprintf(stdout, "checkWinner\n");
  glLoadIdentity();
  glTranslatef((GLfloat) (0), (GLfloat) (0), 0.0f);
  glColor3f(1.0f, 0.5f, 0.5f);
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(20);
  for (int i = 0; i < 3; ++i) {
    int t = field[3 * i];
    for (int j = 0; j < 3; ++j) if (field[3 * i + j] != t) t = -1;
    if (t >= 0) {
      glBegin(GL_LINES);
        glVertex2f(0.4, 3 - (0.6 + i));
        glVertex2f(0.6 + 2, 3 - (0.4 + i));
      glEnd();
    }
  }
  for (int j = 0; j < 3; ++j) {
    int t = field[j];
    for (int i = 0; i < 3; ++i) if (field[3 * i + j] != t) t = -1;
    if (t >= 0) {
      glBegin(GL_LINES);
        glVertex2f(0.4 + j, 3 - (0.6));
        glVertex2f(0.6 + j, 3 - (0.4 + 2));
      glEnd();
    }
  }
  {
    int t = field[0];
    for (int i = 0; i < 3; ++i) if (field[3 * i + i] != t) t = -1;
    if (t >= 0) {
      glBegin(GL_LINES);
        glVertex2f(0.4 , 3 - (0.6));
        glVertex2f(0.6 + 2, 3 - (0.4 + 2));
      glEnd();
    }
  }
  {
    int t = field[2];
    for (int i = 0; i < 3; ++i) if (field[3 * i + 2 - i] != t) t = -1;
    if (t >= 0) {
      glBegin(GL_LINES);
        glVertex2f(0.4 + 2 , 3 - (0.4));
        glVertex2f(0.6, 3 - (0.6 + 2));
      glEnd();
    }
  }
  glDisable(GL_LINE_SMOOTH);
}

static void drawCross(void) {
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(10);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, cross_vertices);
  glDrawArrays(GL_LINES, 0, 4);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisable(GL_LINE_SMOOTH);
}

static void drawCircle(void) {
  GLint nos = 100; // number of sides in "circle"
  GLfloat dpi = 2.0f * M_PI; // double PI
  GLfloat cvx[nos], cvy[nos], cvz[nos]; // circleVerticesX, circleVerticesY, circleVerticesZ
  for (int i=0; i < nos; ++i) {
    cvx[i] = 0.45f * cos(i * dpi / nos);
    cvy[i] = 0.45f * sin(i * dpi / nos);
    cvz[i] = 0;
  }
  GLfloat allcv[nos*2*3]; // All Circle Vertices
  for (int i = 0; i < nos; ++i) {
    allcv[2*3*i] = cvx[i];
    allcv[2*3*i + 1] = cvy[i];
    allcv[2*3*i + 2] = cvz[i];
    allcv[2*3*i + 3] = cvx[(i+1) % nos];
    allcv[2*3*i + 4] = cvy[(i+1) % nos];
    allcv[2*3*i + 5] = cvz[(i+1) % nos];
  }
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(10);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, allcv);
  glDrawArrays(GL_LINES, 0, nos*2);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisable(GL_LINE_SMOOTH);
}

static void drawField(void) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 3.0, 0.0, 3.0, 0.0, 1.0);

  glMatrixMode(GL_MODELVIEW);

  glColor3f(1.0f, 1.0f, 0.6f);

  for (int i = 0; i < 3; ++i)
  for (int j = 0; j < 3; ++j) {
    int t = field[3*(2-i) + j];
    glLoadIdentity();
    glTranslatef((GLfloat) (0.5 + j), (GLfloat) (0.5 + i), 0.0f);
    if (t == 1) drawCross(); else if (t == 0) drawCircle();
  }

  checkWinner();
}

static void framebufferSizeFun(GLFWwindow* window, int w, int h) {
  width  = w;
  height = h > 0 ? h : 1;
  do_redraw = 1;
}

static void windowRefreshFun(GLFWwindow* window) {
  drawField();
  glfwSwapBuffers(window);
  do_redraw = 0;
}

static void cursorPosFun(GLFWwindow* window, double x, double y) {
  int wnd_width, wnd_height, fb_width, fb_height;
  double scale;

  glfwGetWindowSize(window, &wnd_width, &wnd_height);
  glfwGetFramebufferSize(window, &fb_width, &fb_height);

  scale = (double) fb_width / (double) wnd_width;

  x *= scale;
  y *= scale;

  xpos = x;
  ypos = y;
}

static void mouseButtonFun(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      // ...
    } else {
      int ind = 3 * ((int)(ypos * 3 / 500)) + ((int)(xpos * 3 / 500));
      if (field[ind] < 0) {
        field[ind] = turn;
        turn = 1 - turn;
      }
    }
  }

  do_redraw = 1;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int main(void) {
  GLFWwindow* window;
  if (!glfwInit()) { fprintf(stderr, "Failed to initialize GLFW\n"); exit(EXIT_FAILURE); }

  window = glfwCreateWindow(500, 500, "Tic tac toe", NULL, NULL);
  if (!window) { fprintf(stderr, "Failed to open GLFW window\n"); glfwTerminate(); exit(EXIT_FAILURE); }

  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  glfwSetWindowPos(window, mode->width / 2 - 250, mode->height / 2 - 250);

  glfwSetFramebufferSizeCallback(window, framebufferSizeFun);
  glfwSetWindowRefreshCallback(window, windowRefreshFun);
  glfwSetCursorPosCallback(window, cursorPosFun);
  glfwSetMouseButtonCallback(window, mouseButtonFun);
  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);
  glfwSwapInterval(1);

  glfwGetFramebufferSize(window, &width, &height);
  framebufferSizeFun(window, width, height);

  for (;;) {
    if (do_redraw) windowRefreshFun(window);

    glfwWaitEvents();

    if (glfwWindowShouldClose(window)) break;
  }

  glfwTerminate();

  exit(EXIT_SUCCESS);
}



























