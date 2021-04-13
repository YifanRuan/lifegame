/**
 * @file life.c
 * @author ���㷲
 * @brief ������ϷԴ�ļ�
 * @version 1.0
 * @date 2020-12-26
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief �ַ�����󳤶ȡ�
 *
 */
#define LEN 1024

/**
 * @brief ��ͼ��󳤿�
 *
 */
#define KMAX 120

#define HELP "\\h"
#define LOAD "\\l"
#define SAVE "\\s"
#define DESIGN "\\d"
#define GENERATE "\\g"
#define RUN "\\r"
#define EXIT "\\e"
#define QUIT "\\q"
#define PRINT "\\p"
#define END "end"
#define EMPTY ""

/**
 * @brief ��ͼ������
 *
 */
int row = 0;

/**
 * @brief ��ͼ������
 *
 */
int col = 0;

/**
 * @brief ��ǣ��жϳ������Ƿ����е�ͼ��
 *
 */
int is_map = 0;

/**
 * @brief ȫ�ֵ�ͼ���顣
 *
 */
int map[KMAX][KMAX];

/**
 * @brief ���ڴ��ϸ��������
 *
 */
int alive_neighbors[KMAX][KMAX];

void get_input(char *);

void help(void);

void welcome(void);

void get_command(char *, char *, char *);

void load_map(char *);

void save_map(char *);

void convert_lower_case(char *);

void generate_next_status(void);

void print_map(void);

void design_map(void);

void auto_run(void);

void is_map_error(void);

int main() {
  system("cls");
  welcome();
  char cmd[LEN], buff[LEN], filename[LEN];
  while (1) {
    printf("\n[I] -> ");
    get_input(cmd);
    get_command(cmd, buff, filename);
    if (strcmp(buff, HELP) == 0 && strcmp(filename, EMPTY) == 0) {
      help();
    } else if (strcmp(buff, LOAD) == 0) {
      load_map(filename);
    } else if (strcmp(buff, SAVE) == 0) {
      save_map(filename);
    } else if (strcmp(buff, DESIGN) == 0 && strcmp(filename, EMPTY) == 0) {
      design_map();
    } else if (strcmp(buff, GENERATE) == 0 && strcmp(filename, EMPTY) == 0) {
      generate_next_status();
      print_map();
    } else if (strcmp(buff, RUN) == 0 && strcmp(filename, EMPTY) == 0) {
      auto_run();
    } else if (strcmp(buff, EXIT) == 0 && strcmp(filename, EMPTY) == 0) {
      printf("This command is only available in auto_run mode.\n");
    } else if (strcmp(buff, QUIT) == 0 && strcmp(filename, EMPTY) == 0) {
      printf("This command is only available in design mode.\n");
    } else if (strcmp(buff, PRINT) == 0 && strcmp(filename, EMPTY) == 0) {
      print_map();
    } else if (strcmp(buff, END) == 0 && strcmp(filename, EMPTY) == 0) {
      printf("See you next time!\n");
      break;
    } else if (strcmp(buff, EMPTY) == 0) {
      continue;
    } else {
      printf("error: command not found: %s %s\n", buff, filename);
    }
    printf("\n");
  }
  return 0;
}

/**
 * @brief ��ȡһ���������ַ��������Ȳ�����1024��
 *
 * @param buf �����ַ������λ��
 */
void get_input(char *buf) {
  fgets(buf, LEN, stdin);
  buf[strlen(buf) - 1] = '\0';
}
/**
 * @brief ��ʾ�������档
 *
 */
void help() {
  printf("    [\\h]    print command [h]ints\n");
  printf("    [\\p]    [p]rint current map\n");
  printf("    [\\l <filename>]  [l]oad map from a local file\n");
  printf("    [\\s <filename>]  [s]ave map to local\n");
  printf("    [\\d]    enter [d]esign mode\n");
  printf("    [\\q]    [q]uit design mode\n");
  printf("    [\\g]    [g]enerate next generation of life\n");
  printf("    [\\r]    enter auto_[r]un mode\n");
  printf("    [\\e]    [e]xit auto_run mode\n");
  printf("    [end]   [end] the game\n");
}

/**
 * @brief ��ʾ��ʼ��ӭ���档
 *
 */
void welcome() {
  char *s = "************************************************\n";
  printf("%s%s", s, s);
  printf("                GAME OF LIFE\n\n");
  printf("          Welcome to the game of life.\n");
  printf("You'll experience the joy of Cellular Automaton.\n\n");
  printf("                ARE YOU READY\n\n");
  printf("%s%s", s, s);
  help();
}

/**
 * @brief
 * ��һ���ַ����ֽ�Ϊ�������֡���һ������Ϊȥ���հ��ַ����ͷ������һ���հ��ַ���β��֮ǰ���ڶ�������Ϊ��һ���հ��ַ���β��ȥ���հ��ַ�֮ǰ�������ڷֽ������ȡ���������ȡ����ֽ�����ʱ�������ڴ�����ո���ļ�����·��������ȡ��������ʱ����ͨ��һϵ���ж���ȷ������һ��������������
 *
 * @param cmd ��������ͨ��Ϊ��ȡ��һ�����ַ�����
 * @param buf �ֽ�ĵ�һ���ַ���
 * @param filename �ֽ�ĵڶ����ַ���
 */
void get_command(char *cmd, char *buf, char *filename) {
  convert_lower_case(cmd);
  int t = 0, len = (int)strlen(cmd), i = 0, j = 0;
  while (t < len && (cmd[t] == ' ' || cmd[t] == '\t')) ++t;
  while (t < len && cmd[t] != ' ' && cmd[t] != '\t') buf[i++] = cmd[t++];
  buf[i] = '\0';
  while (t < len && (cmd[t] == ' ' || cmd[t] == '\t')) ++t;
  while (t < len) filename[j++] = cmd[t++];
  j--;
  while (j >= 0 && filename[j] == ' ' || filename[j] == '\t') j--;
  filename[j + 1] = '\0';
}

/**
 * @brief ��������ַ���s�е���ĸȫת��ΪСд��
 *
 * @param s ��Ҫת�����ַ���
 */
void convert_lower_case(char *s) {
  int len = (int)strlen(s);
  for (int i = 0; i < len; ++i) {
    if (s[i] >= 'A' && s[i] <= 'Z') {
      s[i] += 32;
    }
  }
}

/**
 * @brief
 * ���ر��ص�ͼ�������ϵͳԭ�������ԭ���޷����أ������ļ���������ļ����������������Ϸ��򳬹���Χ���������ڸ�ϸ���Ĵ���������ȡ������������0���ȡΪ�������ȡΪ��������ȡ���Ѿ���ȡ�������ļ�β������ȡ���ʱ��ʣ��ϸ��Ĭ��������
 *
 * @param filename ��Ҫ���ص��ļ���
 */
void load_map(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("load_map: error: no such file\n");
    fclose(fp);
    return;
  }
  int x = 0, y = 0;
  fscanf(fp, "%d%d", &x, &y);
  if (x <= 0 || y <= 0) {
    printf("load_map: error: illegal map\n");
    return;
  }
  if (x >= KMAX || y >= KMAX) {
    printf("load_map: error: map is too large\n");
    return;
  }
  row = x, col = y;
  double buf;
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      buf = 0;
      fscanf(fp, "%lf", &buf);
      if (buf > 0) {
        map[i][j] = 1;
      } else {
        map[i][j] = 0;
      }
    }
  }
  fclose(fp);
  is_map = 1;
  printf("row = %d, column = %d\n", row, col);
  printf("loading complete\n");
}

/**
 * @brief �����ͼ�����ء����޵�ͼ�򱨴��˳���������ϵͳԭ�򱣴�ʧ�ܡ�
 *
 * @param filename ��Ҫ������ļ���
 */
void save_map(char *filename) {
  if (!is_map) {
    is_map_error();
    return;
  }
  FILE *fp = fopen(filename, "w");
  if (fp == NULL) {
    printf("save_map: error: failed to save file\n");
    fclose(fp);
    return;
  }
  fprintf(fp, "%d %d\n", row, col);
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      fprintf(fp, "%d ", map[i][j]);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
  printf("saving successfully\n");
}

/**
 * @brief �����ͼ��ĳ����Χ���ϸ�����������Ա߽�����������ۡ�
 *
 * @param x x����
 * @param y y����
 * @return int �õ���Χ���ϸ��������
 */
int count_alive_neighbors(int x, int y) {
  int alive = 0;
  for (int i = x - 1; i <= x + 1; ++i) {
    if (i < 0 || i >= row) continue;
    for (int j = y - 1; j <= y + 1; ++j) {
      if (j < 0 || j >= col || (i == x && j == y)) continue;
      alive += map[i][j];
    }
  }
  return alive;
}

/**
 * @brief
 * ������һ��ϸ��ͼ���ȼ����ϸ����Χ���ϸ�������ٸ�����Ϸ�������ϸ��ͼ��
 *
 */
void generate_next_status() {
  if (!is_map) {
    is_map_error();
    return;
  }
  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++)
      alive_neighbors[i][j] = count_alive_neighbors(i, j);
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      if (map[i][j] == 1 &&
          (alive_neighbors[i][j] < 2 || alive_neighbors[i][j] > 3)) {
        map[i][j] = 0;
      } else if (map[i][j] == 0 && alive_neighbors[i][j] == 3) {
        map[i][j] = 1;
      }
    }
  }
}

/**
 * @brief ��ӡ��ͼ�������е�ͼ�����ӡ����������޵�ͼ����
 *
 */
void print_map() {
  if (!is_map) {
    is_map_error();
    return;
  }
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf(map[i][j] > 0 ? "�� " : "�� ");
    }
    printf("\n");
  }
}

/**
 * @brief �����ͼ���ģʽ������ѭ�������û����룬�� is_design
 * �ж�����Ϊ��������������λ�á��û�������ΪС��120���������зǷ������������������������������ƽ�������ʾ�û����档
 *
 */
void design_map() {
  system("cls");
  char buf[LEN], s1[LEN], s2[LEN];
  int len1, len2, x, y, is_digit;
  printf("Enter design mode.\n");
  int is_design = 0;
  while (1) {
    if (!is_design) {
      printf(
          "Set row and column: (EX: 5 4) or use [\\q] to quit (less than %d)\n",
          KMAX);
    } else {
      printf("\n[D] (use [\\q] to quit) -> ");
    }
    get_input(buf);
    get_command(buf, s1, s2);
    if (strcmp(s1, QUIT) == 0 && strcmp(s2, EMPTY) == 0) {
      break;
    }
    is_digit = 1, len1 = (int)strlen(s1), len2 = (int)strlen(s2);
    if (len1 == 0) {
      continue;
    }
    if (len2 == 0) {
      is_digit = 0;
    }
    for (int i = 0; i < len1 && is_digit; ++i) {
      if (s1[i] < '0' || s1[i] > '9') {
        is_digit = 0;
      }
    }
    for (int i = 0; i < len2 && is_digit; ++i) {
      if (s2[i] < '0' || s2[i] > '9') {
        is_digit = 0;
      }
    }
    if (!is_digit) {
      printf("design_map: error: format error\n");
      continue;
    }
    x = 0, y = 0;
    for (int i = len1 - 1; i >= 0; i--) {
      x += (s1[i] - '0') * (int)pow(10, len1 - 1 - i);
    }
    for (int i = len2 - 1; i >= 0; i--) {
      y += (s2[i] - '0') * (int)pow(10, len2 - 1 - i);
    }
    if (!is_design) {
      if (x <= 0 || y <= 0) {
        printf("design_map: error: illegal number\n");
        continue;
      }
      if (x >= KMAX || y >= KMAX) {
        printf("design_map: error: number too large\n");
        continue;
      }
    } else if (x >= row || y >= col) {
      printf("design_map: error: illegal number\n");
      continue;
    }
    if (!is_design) {
      is_design = 1;
      row = x, col = y;
      for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
          map[i][j] = 0;
        }
      }
      printf("Set alive cells. (EX: 0 0)\n");
      is_map = 1;
      print_map();
    } else {
      map[x][y] = 1;
    }
  }
  system("cls");
  printf("--> You have quited the design mode.\n");
  if (is_design) {
    print_map();
    printf("(Use [\\s <filename>] to save map to local.)\n");
  }
}

/**
 * @brief �����Զ�����ģʽ���޵�ͼ���˳����� is_run
 * �������״̬��1Ϊ�������У�0Ϊ������ͣ���Զ����м��2�룬ÿ������ǰ����������
 * ENTER ����ͣ����ͣʱ���� ENTER ���������� [\e] �˳���ģʽ������Ϊ�������
 *
 */
void auto_run() {
  if (!is_map) {
    is_map_error();
    printf("Exit auto_run mode...\n");
    return;
  }
  int is_run = 1;
  char buff[LEN], s1[LEN], s2[LEN];
  while (1) {
    if (is_run) {
      system("cls");
      generate_next_status();
      printf("Press ENTER to suspend.\n");
      print_map();
      sleep(2);
      if (kbhit()) {
        if (getch() == '\r') {
          is_run = 0;
        }
      }
    } else {
      printf("\nPress ENTER to restart or use [\\e] to exit auto_run mode.\n");
      get_input(buff);
      get_command(buff, s1, s2);
      if (strcmp(s1, EXIT) == 0 && strcmp(s2, EMPTY) == 0) {
        system("cls");
        printf("--> You have returned to the normal mode.\n");
        break;
      } else if (strcmp(s1, EMPTY) == 0) {
        is_run = 1;
      } else {
        printf("auto_run: error: input error");
      }
    }
  }
}

/**
 * @brief ����޵�ͼ������Ϣ������ʾ�û��ɵ����ͼ���Զ����ͼ��
 *
 */
void is_map_error() {
  printf("error: no map yet!\n");
  printf("Use [\\l <filename] to load an existing file.\n");
  printf("Or use [\\d] to design a new map.\n");
}
