#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "search.h"

GtkWidget *see_credits;
GtkWidget *window_game;
GtkWidget *window;
GtkWidget *window_n_par;
GtkWidget *window_par;
GtkWidget *window_nickname;
GtkWidget *window_congrats;
GtkWidget *window_highscores;
GtkWidget *window_instructions;
GtkWidget *score;
GtkWidget *player;
GtkWidget *highScore1;
GtkWidget *highScore2;
GtkWidget *highScore3;
GtkWidget *highScore4;
GtkWidget *highScore5;
GtkWidget *instructions;
GtkEntry *nick_entry;
GtkEntryBuffer *entry_buffer;
GtkImage *images[30];
GtkBuilder *builder;
FILE *file_highscores;

int scores[5];
int card_temp = 0;
int points = 0;
int imageIndex;
int matrix[6][5][2];
int hits = 0;
char nickname[30];
char nicks[5][30];

void read_highscores ();
void link_images ();
void load_images ();
void load_image_back ();
void fill_matrix ();
void on_btn_ok_n_par_clicked ();
void on_btn_ok_par_clicked ();
void on_btn_nickname_ok_clicked ();
void on_btn_congrats_ok_clicked ();
void on_btn_exit_clicked ();
void on_btn_highscores_clicked ();
void on_btn_highscores_close_clicked ();
void on_btn_instructions_close_clicked ();
void on_btn_playgame_exit_clicked ();
void on_btn_instructions_clicked ();
void on_window_nickname_destroy ();
void on_window_congrats_destroy ();
void on_window_highscores_destroy ();

int main(int argc, char *argv[]) {

    srand(time(NULL));

    file_highscores = fopen(".highscores.txt", "r+");

    read_highscores ();

    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "src/window_main.glade.xml", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    see_credits = GTK_WIDGET(gtk_builder_get_object(builder, "window_credits"));
	window_game = GTK_WIDGET(gtk_builder_get_object(builder, "window_playgame"));
	window_par = GTK_WIDGET(gtk_builder_get_object(builder, "window_confirm_par"));
	window_n_par = GTK_WIDGET(gtk_builder_get_object(builder, "window_confirm_n_par"));
	window_congrats = GTK_WIDGET(gtk_builder_get_object(builder, "window_congrats"));
	window_nickname = GTK_WIDGET(gtk_builder_get_object(builder, "window_nickname"));
	window_highscores = GTK_WIDGET(gtk_builder_get_object(builder, "window_highscores"));
	window_instructions = GTK_WIDGET(gtk_builder_get_object(builder, "window_instructions"));
	nick_entry = GTK_WIDGET(gtk_builder_get_object(builder, "txt_nick_entry"));
	score = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_score"));
	player = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_player"));
	highScore1 = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_highscore_1"));
	highScore2 = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_highscore_2"));
	highScore3 = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_highscore_3"));
	highScore4 = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_highscore_4"));
	highScore5 = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_highscore_5"));
	instructions = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_instructions"));

 
	gtk_widget_hide_on_delete(window_game);
	gtk_widget_hide_on_delete(window_nickname);

    link_images();

    g_object_unref(builder);
    gtk_widget_show(window);                
    gtk_main();

    //Limpa o arquivo .highscores.txt
    system("> .highscores.txt");

    //Escreve o ranking no arquivo .highscores.txt
    for(int i = 0; i < 5; i++){

    	char print[50];
    	char temp[3];

    	strcpy(print, "echo \"");
    	sprintf(temp, "%d", scores[i]);
    	strcat(print, temp);
    	strcat(print, "\t");
    	strcat(print, nicks[i]);
    	strcat(print, "\"");
    	strcat(print, " >> .highscores.txt");
    	system(print);
    }

    fclose(file_highscores);

    return 0;
}

//Le o ranking atual no arquivo .highscores.txt
void read_highscores () {

	int temp1 = 0;
	int temp2 = 0;
	char temp[50];
	int j;
	int i;
	int key;

	for(int i = 0; i < 10; i++){

        fscanf(file_highscores, "%s", temp);

        if (i % 2 == 0){
        	scores[temp1] = atoi(temp);
        	//printf("%d\t", scores[temp1]);
        	temp1++;
        } else {
        	strcpy(nicks[temp2], temp);
        	//printf("%s\n", nicks[temp2]);
        	temp2++;
        }
    }
	
	for(j = 1; j < 5; j++) {
		
		key = scores[j];
		i = j - 1;
		strcpy(temp, nicks[j]);
		
		while(i >= 0 && scores[i] < key) {
			scores[i + 1] = scores[i];
			strcpy(nicks[i + 1], nicks[i]);
			i = i - 1;
		}

		strcpy(nicks[i + 1], temp);
		scores[i + 1] = key;
	}
}

//Linkando as imagens
void link_images () {

    for (int i = 0; i < 30; i++) {

        char imageIndex[30];
        char buffer[3];

        strcpy(imageIndex, "image");
        sprintf(buffer, "%d", i+1);
        strcat(imageIndex, buffer);

        images[i] = GTK_IMAGE(gtk_builder_get_object(builder, imageIndex));

    }

}

//Carregando as imagens
void load_images () {

    int k = 0;

    for(int i = 0; i < 6; i++){

        for(int q = 0; q < 5; q++){

            if(matrix[i][q][1] == 0){
                
                char imageName[30];
                char buffer[3];

                strcpy(imageName, "src/images/card_");
                sprintf(buffer, "%d", matrix[i][q][0]);
                strcat(imageName, buffer);
                strcat(imageName, ".png");

                gtk_image_set_from_file(images[k], imageName);

            } else {
                
                gtk_image_set_from_file(images[k], "src/images/card_back.png");

            }
         
            k++;

        }

    }

}

void fill_matrix () {

    int temp; 
    int numbers[15];
    bool exit = false;
    int i = 0;

    while(1){


        if(i == 15)
            break;

        temp = rand() % 52 + 1;

        if(!sequentialsearch(numbers, 15, temp)){
            numbers[i] = temp;
            i++;
        }

    }

    for(int i = 0; i < 6; i++){

        for(int q = 0; q < 5; q++){
            matrix[i][q][0] = NULL;
            matrix[i][q][1] = 1;
        }

    }

    for(int i = 0; i < 15; i++){

        while(1){

            int position = rand() % 30; 

            if(matrix[position / 5][position % 5][0] == NULL){
                matrix[position / 5][position % 5][0] = numbers[i];
                break;
            }

        }
        exit = false;

        while(1){

            int position = rand() % 30; 

            if(matrix[position / 5][position % 5][0] == NULL){
                matrix[position / 5][position % 5][0] = numbers[i];
                break;
            }

        }

        exit = false;
    }


}

void on_btn_instructions_clicked () {

	gtk_widget_show(window_instructions);
	gtk_label_set_text(GTK_LABEL(instructions), "O jogo da memória é um clássico jogo formado por peças que apresentam uma figura em um dos lados. \nCada figura se repete em duas peças diferentes. Para começar o jogo, as peças são postas com as figuras voltadas\n para baixo, para que não possam ser vistas. O jogador deve, na sua vez, virar duas peças.\n Caso as figuras sejam iguais, ele recolhe consigo esse par.\n Se forem peças diferentes, estas são viradas novamente, e a vez é passada ao participante seguinte.\n Ganha o jogo quem tiver descoberto mais pares, quando todos eles tiverem sido recolhidos.\n");

}

void on_btn_instructions_close_clicked () {

	gtk_widget_hide(window_instructions);

}

//Fechar janela no X
void on_window_main_destroy() {

    gtk_main_quit();  

}

void on_window_nickname_destroy () {

	gtk_main_quit();

}

void on_window_congrats_destroy () {

	on_btn_congrats_ok_clicked();

}

void on_window_confirm_n_par_destroy () {

    on_btn_ok_n_par_clicked();

}

void on_window_confirm_par_destroy () {

    on_btn_ok_par_clicked();

}

void on_window_playgame_destroy () {

	gtk_widget_show(window);
	gtk_widget_hide(window_game);

}

void on_btn_playgame_exit_clicked () {

	gtk_widget_show(window);
 	gtk_widget_hide(window_game);

}

void on_btn_highscores_clicked () {

	char temp[50];
	char buffer[3];

	strcpy(temp, "#1\t|\t");
	sprintf(buffer, "%d", scores[0]);
	strcat(temp, buffer);
	strcat(temp, "\t\t|\t");
	strcat(temp, nicks[0]);

	gtk_label_set_text(GTK_LABEL(highScore1), temp);

	strcpy(temp, "#2\t|\t");
	sprintf(buffer, "%d", scores[1]);
	strcat(temp, buffer);
	strcat(temp, "\t\t|\t");
	strcat(temp, nicks[1]);

	gtk_label_set_text(GTK_LABEL(highScore2), temp);

	strcpy(temp, "#3\t|\t");
	sprintf(buffer, "%d", scores[2]);
	strcat(temp, buffer);
	strcat(temp, "\t\t|\t");
	strcat(temp, nicks[2]);

	gtk_label_set_text(GTK_LABEL(highScore3), temp);

	strcpy(temp, "#4\t|\t");
	sprintf(buffer, "%d", scores[3]);
	strcat(temp, buffer);
	strcat(temp, "\t\t|\t");
	strcat(temp, nicks[3]);

	gtk_label_set_text(GTK_LABEL(highScore4), temp);

	strcpy(temp, "#5\t|\t");
	sprintf(buffer, "%d", scores[4]);
	strcat(temp, buffer);
	strcat(temp, "\t\t|\t");
	strcat(temp, nicks[4]);

	gtk_label_set_text(GTK_LABEL(highScore5), temp);

	gtk_widget_show(window_highscores);
}

void on_btn_highscores_close_clicked () {

	gtk_widget_hide(window_highscores);

}

void on_btn_exit_clicked () {

	gtk_main_quit();

}

void on_btn_congrats_ok_clicked () {

	int j;
	int i;
	int key;
	char temp[30];

	if (points > scores[4]) {
		scores[4] = points;
		strcpy(nicks[4], nickname);
	}

	for(j = 1; j < 5; j++) {
		
		key = scores[j];
		i = j - 1;
		strcpy(temp, nicks[j]);
		
		while(i >= 0 && scores[i] < key) {
			scores[i + 1] = scores[i];
			strcpy(nicks[i + 1], nicks[i]);
			i = i - 1;
		}

		strcpy(nicks[i + 1], temp);
		scores[i + 1] = key;
	}

	gtk_widget_show(window);
	gtk_widget_hide(window_congrats);
	gtk_widget_hide(window_game);

}

void on_btn_playgame_clicked () {

	fill_matrix(matrix);
    load_images(matrix);

    points = 0;

    gtk_widget_show(GTK_WIDGET(window_nickname));
}

void on_btn_creditos_clicked () {

    gtk_dialog_run(GTK_DIALOG(see_credits));
    gtk_widget_hide(see_credits);

}

void on_btn_nickname_ok_clicked () {

	char temp[50];

	points = 0;

	entry_buffer = gtk_entry_get_buffer (nick_entry);
	strcpy(nickname, gtk_entry_buffer_get_text (entry_buffer));

	strcpy(temp, "Jogador: ");
	strcat(temp, nickname);
	gtk_label_set_text(GTK_LABEL(player), temp);
	gtk_label_set_text(GTK_LABEL(score), "Pontos: 0");

	gtk_widget_show(GTK_WIDGET(window_game));
	gtk_widget_hide(window_nickname);
}


void on_btn_ok_n_par_clicked () {

    points--;
    char points_c[30];
    char buffer_lbl[30];
    strcpy(points_c, "Pontos: ");
    sprintf(buffer_lbl, "%d", points);
    strcat(points_c, buffer_lbl);
    gtk_label_set_text(GTK_LABEL(score), points_c);
    gtk_image_set_from_file(images[imageIndex], "src/images/card_back.png");
    gtk_image_set_from_file(images[card_temp-1], "src/images/card_back.png");
    card_temp = 0;
    gtk_widget_hide(window_n_par);

}

void on_btn_ok_par_clicked () {

	hits++;
    points++;
    char points_c[30];
    char buffer_lbl[30];
    strcpy(points_c, "Pontos: ");
    sprintf(buffer_lbl, "%d", points);
    strcat(points_c, buffer_lbl);
    gtk_label_set_text(GTK_LABEL(score), points_c);

    if (hits >= 15) {
    	gtk_widget_show(window_congrats);
    }

    gtk_widget_hide(window_par);

}

void on_btn_1_clicked () {

    imageIndex = 0;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_2_clicked () {

    imageIndex = 1;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_3_clicked () {

    imageIndex = 2;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_4_clicked () {

    imageIndex = 3;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }

    }
}

void on_btn_5_clicked () {

    imageIndex = 4;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_6_clicked () {

    imageIndex = 5;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_7_clicked () {

    imageIndex = 6;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_8_clicked () {

    imageIndex = 7;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_9_clicked () {

    imageIndex = 8;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_10_clicked () {

    imageIndex = 9;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_11_clicked () {

    imageIndex = 10;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_12_clicked () {

    imageIndex = 11;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_13_clicked () {

    imageIndex = 12;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_14_clicked () {

    imageIndex = 13;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_15_clicked () {

    imageIndex = 14;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_16_clicked () {

    imageIndex = 15;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_17_clicked () {

    imageIndex = 16;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_18_clicked () {

    imageIndex = 17;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_19_clicked () {

    imageIndex = 18;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_20_clicked () {

    imageIndex = 19;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_21_clicked () {

    imageIndex = 20;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_22_clicked () {

    imageIndex = 21;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_23_clicked () {

    imageIndex = 22;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_24_clicked () {

    imageIndex = 23;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_25_clicked () {

    imageIndex = 24;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_26_clicked () {

    imageIndex = 25;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_27_clicked () {

    imageIndex = 26;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_28_clicked () {

    imageIndex = 27;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_29_clicked () {

    imageIndex = 28;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}

void on_btn_30_clicked () {

    imageIndex = 29;

    if (matrix[imageIndex/5][imageIndex%5][1] == 0) {
        return;
    }

    if (card_temp == imageIndex+1) {
        return;
    }

    matrix[imageIndex/5][imageIndex%5][1] = 0;

    if (card_temp == 0) {

        char imageName[30];
        char buffer[3];

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);
        card_temp = imageIndex+1;

    } else {

        char imageName[30];
        char buffer[3];

        matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 0;

        strcpy(imageName, "src/images/card_");
        sprintf(buffer, "%d", matrix[imageIndex/5][imageIndex%5][0]);
        strcat(imageName, buffer);
        strcat(imageName, ".png");

        gtk_image_set_from_file(images[imageIndex], imageName);

        if (matrix[imageIndex/5][imageIndex%5][0] == matrix[(card_temp-1)/5][(card_temp-1)%5][0]) {

            gtk_widget_show(GTK_WIDGET(window_par));
            card_temp = 0;

        } else {

            gtk_widget_show(GTK_WIDGET(window_n_par));
            matrix[imageIndex/5][imageIndex%5][1] = 1;
            matrix[(card_temp-1)/5][(card_temp-1)%5][1] = 1;

        }
    }
}