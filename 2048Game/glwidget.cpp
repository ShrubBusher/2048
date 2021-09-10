#include "glwidget.h"
#include "mainwindow.h"
#include <iostream>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent){}

static int score = 0;

static const int field_size = 8;
static const float grid_spacing = 12.5f;
static const float half_grid_spacing = grid_spacing / 2.0f;
static const float tile_size = 100.0f;
static const float field_half = (tile_size + grid_spacing) * (field_size / 2.0f) + (grid_spacing / 2.0f);

static bool first = true;
static bool move_was_made = false;
static bool field_changed = false;
static bool game_end = false;

static GLuint textures[11];

int array_field[field_size][field_size]//{{0}};
    {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

int array_blank[field_size][field_size] {{0}};
int* array_random[field_size * field_size] {0};

int GLWidget::score_changed()
{
  return score;
}

bool GLWidget::game_end_changed()
{
  return game_end;
}

bool field_check(int (*arr)[field_size])
{
  bool check = false;
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if (arr[i][j] == arr[(i == field_size - 1) ? i : i + 1][j] || arr[i][j] == arr[i][(j == 3) ? j : j + 1]) { check = true; }
    }
  }

  return check;
}

void ArrayRotate(int (*arr)[field_size], bool to_right);
bool CheckFor2048(int field[field_size][field_size]);

void MoveArrayUp(int (*arr)[field_size])
{
  ArrayRotate(arr, true);
  std::cout << "MoveArrayUp" << std::endl;
  for (int i = field_size - 1; i > -1; i--)
  {
    for (int j = field_size - 1; j > -1; j--)
    {
      for (int k = 0; k < field_size; k++)
      {
        if (arr[i][k] > 0 && arr[i][k + 1] == 0 && k < field_size - 1) { arr[i][k + 1] = arr[i][k]; arr[i][k] = 0; field_changed = true; }
        else if (arr[i][k] > 0 && arr[i][k - 1] == arr[i][k] && arr[i][k + 1] != arr[i][k] && k > 0 && arr[i][k] % 2 == 0 && arr[i][k - 1] % 2 == 0) { arr[i][k] = arr[i][k - 1] + arr[i][k] + 1; arr[i][k - 1] = 0; field_changed = true; score = score + arr[i][k] - 1; }
        if (arr[i][j] > 0 && arr[i][j - 1] == arr[i][j] && j > 0 && arr[i][j] % 2 == 0 && arr[i][j - 1] % 2 == 0) { arr[i][j] = arr[i][j] + arr[i][j - 1] + 1; arr[i][j - 1] = 0; field_changed = true; score = score + arr[i][j] - 1; }
      }
    }
  }

  for (int i = 0; i < field_size; i++)
  {
    for (int j = 0; j < field_size; j++)
    {
      if (arr[i][j] % 2 != 0) { arr[i][j] = arr[i][j] - 1; }
    }
  }

  move_was_made = true;
  ArrayRotate(arr, false);

  return;
}

void MoveArrayLeft(int (*arr)[field_size])
{
  std::cout << "MoveArrayLeft" << std::endl;
  for (int i = 0; i < field_size; i++)
  {
    for (int j = 0; j < field_size; j++)
    {
      for (int k = field_size - 1; k > -1; k--)
      {
        if (arr[i][k] > 0 && arr[i][k - 1] == 0 && k > 0) { arr[i][k - 1] = arr[i][k]; arr[i][k] = 0; field_changed = true;}
        else if (arr[i][k] > 0 && arr[i][k + 1] == arr[i][k] && arr[i][k - 1] != arr[i][k] && k < field_size - 1 && arr[i][k] % 2 == 0 && arr[i][k + 1] % 2 == 0) { arr[i][k] = arr[i][k + 1] + arr[i][k] + 1; arr[i][k + 1] = 0; field_changed = true; score = score + arr[i][k] - 1; }
        if (arr[i][j] > 0 && arr[i][j + 1] == arr[i][j] && j < field_size - 1 && arr[i][j] % 2 == 0 && arr[i][j + 1] % 2 == 0) { arr[i][j] = arr[i][j] + arr[i][j + 1] + 1; arr[i][j + 1] = 0; field_changed = true; score = score + arr[i][j] - 1;}
      }
    }
  }

  for (int i = 0; i < field_size; i++)
  {
    for (int j = 0; j < field_size; j++)
    {
      if (arr[i][j] % 2 != 0) { arr[i][j] = arr[i][j] - 1; }
    }
  }

  move_was_made = true;

  return;
}

void MoveArrayDown(int (*arr)[field_size])
{
  ArrayRotate(arr, true);
  std::cout << "MoveArrayDown" << std::endl;
  for (int i = 0; i < field_size; i++)
  {
    for (int j = 0; j < field_size; j++)
    {
      for (int k = field_size - 1; k > -1; k--)
      {
        if (arr[i][k] > 0 && arr[i][k - 1] == 0 && k > 0) { arr[i][k - 1] = arr[i][k]; arr[i][k] = 0; field_changed = true;}
        else if (arr[i][k] > 0 && arr[i][k + 1] == arr[i][k] && arr[i][k - 1] != arr[i][k] && k < field_size - 1 && arr[i][k] % 2 == 0 && arr[i][k + 1] % 2 == 0) { arr[i][k] = arr[i][k + 1] + arr[i][k] + 1; arr[i][k + 1] = 0; field_changed = true; score = score + arr[i][k] - 1; }
        if (arr[i][j] > 0 && arr[i][j + 1] == arr[i][j] && j < field_size - 1 && arr[i][j] % 2 == 0 && arr[i][j + 1] % 2 == 0) { arr[i][j] = arr[i][j] + arr[i][j + 1] + 1; arr[i][j + 1] = 0; field_changed = true; score = score + arr[i][j] - 1;}
      }
    }
  }

  for (int i = 0; i < field_size; i++)
  {
    for (int j = 0; j < field_size; j++)
    {
      if (arr[i][j] % 2 != 0) { arr[i][j] = arr[i][j] - 1; }
    }
  }

  move_was_made = true;
  ArrayRotate(arr, false);

  return;
}

void MoveArrayRight(int (*arr)[field_size])
{
  std::cout << "MoveArrayRight" << std::endl;
  for (int i = field_size - 1; i > -1; i--)
  {
    for (int j = field_size - 1; j > -1; j--)
    {
      for (int k = 0; k < field_size; k++)
      {
        if (arr[i][k] > 0 && arr[i][k + 1] == 0 && k < field_size - 1) { arr[i][k + 1] = arr[i][k]; arr[i][k] = 0; field_changed = true; }
        else if (arr[i][k] > 0 && arr[i][k - 1] == arr[i][k] && arr[i][k + 1] != arr[i][k] && k > 0 && arr[i][k] % 2 == 0 && arr[i][k - 1] % 2 == 0) { arr[i][k] = arr[i][k - 1] + arr[i][k] + 1; arr[i][k - 1] = 0; field_changed = true; score = score + arr[i][k] - 1; }
        if (arr[i][j] > 0 && arr[i][j - 1] == arr[i][j] && j > 0 && arr[i][j] % 2 == 0 && arr[i][j - 1] % 2 == 0) { arr[i][j] = arr[i][j] + arr[i][j - 1] + 1; arr[i][j - 1] = 0; field_changed = true; score = score + arr[i][j] - 1; }
      }
      //if (arr[i][j] > 0 && arr[i][j - 1] == arr[i][j] && (j - 1) > -1) { arr[i][j] = arr[i][j] + arr[i][j - 1]; arr[i][j - 1] = 0; }
    }
  }

  for (int i = 0; i < field_size; i++)
  {
    for (int j = 0; j < field_size; j++)
    {
      if (arr[i][j] % 2 != 0) { arr[i][j] = arr[i][j] - 1; }
    }
  }

  move_was_made = true;

  return;
}

void GLWidget::MoveUp()
{
  if(!game_end) { MoveArrayUp(array_field); }
  std::cout << "UpArray   -   worked!" << std::endl;
  move_was_made = true;

  return;
}

void GLWidget::MoveLeft()
{
  if(!game_end) { MoveArrayLeft(array_field); }
  std::cout << "LeftArray   -   worked!" << std::endl;
  move_was_made = true;

  return;
}

void GLWidget::MoveDown()
{
  if(!game_end) { MoveArrayDown(array_field); }
  std::cout << "DownArray   -   worked!" << std::endl;
  move_was_made = true;

  return;
}

void GLWidget::MoveRight()
{
  if(!game_end) { MoveArrayRight(array_field); }
  std::cout << "RightArray   -   worked!" << std::endl;
  move_was_made = true;

  return;
}

void GLWidget::initializeGL()
{
  initializeOpenGLFunctions();
  glClearColor(0.733f, 0.678f, 0.627f, 1.0f); //187, 173, 160 | #bbada0
  glOrtho(-field_half, field_half, field_half, -field_half, -1, 1);

  QImage image[11];
  image[0].load(":/t/images/tile_2.png");
  image[1].load(":/t/images/tile_4.png");
  image[2].load(":/t/images/tile_8.png");
  image[3].load(":/t/images/tile_16.png");
  image[4].load(":/t/images/tile_32.png");
  image[5].load(":/t/images/tile_64.png");
  image[6].load(":/t/images/tile_128.png");
  image[7].load(":/t/images/tile_256.png");
  image[8].load(":/t/images/tile_512.png");
  image[9].load(":/t/images/tile_1024.png");
  image[10].load(":/t/images/tile_2048.png");

  glGenTextures(11, textures);

  for (int i = 0; i < 11 ; i++)
  {

    glBindTexture(GL_TEXTURE_2D, textures[i]);

    image[i] = image[i].convertToFormat(QImage::Format_RGBA8888);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,image[i].width(), image[i].height(),0, GL_RGBA, GL_UNSIGNED_BYTE, image[i].bits() );
  }

}

void GLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);
  DrawField();
  glFlush();
}

void GLWidget::resizeGL(int width, int height)
{
  resize(width, height);
}

void Restart(int (*field)[field_size])
{
  for (int i = 0; i < field_size; i++)
  {
    for (int j = 0; j < field_size; j++)
    {
      field[i][j] = array_blank[i][j];
    }
  }

  first = true;
  move_was_made = false;
  field_changed = false;
  game_end = false;
  score = 0;

  return;
}

void GLWidget::RestartSlot()
{
  Restart(array_field);
  this->update();

  return;
}

bool CheckFor2048(int field[field_size][field_size])
{
  bool check = false;
  for (int i = 0; i < field_size; i++)
  {
    for (int j = 0; j < field_size; j++)
    {
      if (field[i][j] == 2048){ check = true; std::cout << "WIN!!!" << std::endl;}
    }
  }

  return check;
}

void SpawnRandomTile(int (*arr)[field_size])
{
  int elements_added = 0;
  for (int i = 0; i < field_size; i++)
  {
    for (int j = 0; j < field_size; j++)
    {
      if (arr[i][j] == 0) { array_random[elements_added] = &arr[i][j]; elements_added++; }
    }
  }

  if (elements_added < 1) { return; }

  srand(time(0));
  int random = rand() % elements_added;

  *array_random[random] = 1;

  return;
}

void ArrayDebug(int (*arrdebug)[field_size])
{
  for (int i = 0; i < field_size; i++)
  {
    for (int j = 0; j < field_size; j++)
    {
      std::cout << arrdebug[i][j] << ' ';
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  return;
}

void ArrayRotate(int (*arr)[field_size], bool to_right)
{
  //ArrayDebug(arr);
  int arr_new[field_size][field_size]{};
  if(to_right)
  {
    for(int i = 0; i < field_size; i++)
    {
      for(int j = 0; j < field_size; j++)
      {
        arr_new[i][j] = arr[field_size - j - 1][i];
      }
    }
  }

  else
  {
    for(int i = 0; i < field_size; i++)
    {
      for(int j = 0; j < field_size; j++)
      {
        arr_new[i][j] = arr[j][field_size - i - 1];
      }
    }
  }

  for(int i = 0; i < field_size; i++)
  {
    for(int j = 0; j < field_size; j++)
    {
      arr[i][j] = arr_new[i][j];
    }
  }
  //ArrayDebug(arr);

  return;
}

void GLWidget::DrawField()
{
  game_end = CheckFor2048(array_field);
  if (first) { SpawnRandomTile(array_field); SpawnRandomTile(array_field); first = false; }
  if (move_was_made && field_changed) { SpawnRandomTile(array_field); move_was_made = false; field_changed = false; }

  for (int i = 0; i < field_size; i++)
  {
    for (int j = 0; j < field_size; j++)
    {
      float tile_y = (tile_size + grid_spacing) * i - field_half + tile_size + grid_spacing;
      float tile_x = (tile_size + grid_spacing) * j - field_half + grid_spacing;

      switch (array_field[i][j])
      {
      case 1:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glBegin(GL_QUADS);
        //glColor3f(.780, .786, .763);
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex2f(tile_x - half_grid_spacing, tile_y + half_grid_spacing);
        glVertex2f((tile_x + tile_size) + half_grid_spacing, tile_y + half_grid_spacing);
        glVertex2f((tile_x + tile_size) + half_grid_spacing, (tile_y - tile_size) - half_grid_spacing);
        glVertex2f(tile_x - half_grid_spacing, (tile_y - tile_size) - half_grid_spacing);
        array_field[i][j] = array_field[i][j] + 1;
        //glColor3f(.91, .886, .863);
        glColor3f(1,1,1);
        break;
      case 2:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glBegin(GL_QUADS);
        //glColor3f(.91, .886, .863);
        glColor3f(1,1,1);
        break;
      case 4:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glBegin(GL_QUADS);
        //glColor3f(.831, .082, .082);
        glColor3f(1,1,1);
        break;
      case 8:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        glBegin(GL_QUADS);
        //glColor3f(.941, .459, .161);
        glColor3f(1,1,1);
        break;
      case 16:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[3]);
        glBegin(GL_QUADS);
        //glColor3f(1.0, .867, .20);
        glColor3f(1,1,1);
        break;
      case 32:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[4]);
        glBegin(GL_QUADS);
        //glColor3f(.745, .91, .635);
        glColor3f(1,1,1);
        break;
      case 64:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[5]);
        glBegin(GL_QUADS);
        //glColor3f(.278, .612, .55);
        glColor3f(1,1,1);
        break;
      case 128:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[6]);
        glBegin(GL_QUADS);
        //glColor3f(.078, .49, .639);
        glColor3f(1,1,1);
        break;
      case 256:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[7]);
        glBegin(GL_QUADS);
        //glColor3f(.086, 0, .651);
        glColor3f(1,1,1);
        break;
      case 512:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[8]);
        glBegin(GL_QUADS);
        //glColor3f(.416, .059, .671);
        glColor3f(1,1,1);
        break;
      case 1024:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[9]);
        glBegin(GL_QUADS);
        //glColor3f(.651, .039, .431);
        glColor3f(1,1,1);
        break;
      case 2048:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[10]);
        glBegin(GL_QUADS);
        //glColor3f(0,0,0);
        glColor3f(1,1,1);
        break;
      default:
        glBegin(GL_QUADS);
        glColor3f(.82, .765, .714);
        break;
      }

      glTexCoord2f(0.0f, 1.0f); glVertex2f(tile_x, tile_y);
      glTexCoord2f(1.0f, 1.0f); glVertex2f(tile_x + tile_size, tile_y);
      glTexCoord2f(1.0f, 0.0f); glVertex2f(tile_x + tile_size, tile_y - tile_size);
      glTexCoord2f(0.0f, 0.0f); glVertex2f(tile_x, tile_y - tile_size);

      glBindTexture(GL_TEXTURE_2D, 0);

      glEnd();
      glDisable(GL_TEXTURE_2D);
    }
  }

  ArrayDebug(array_field);

  return;
}
