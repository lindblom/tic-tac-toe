/*#######################################################################
#///////////////////////////////////////////////////////////////////////#
#/////////|-------------------[TIC TAC TOE]-------------------|/////////#
#/////////| Developer: Christopher Lindblom [BMCE]			  |/////////#
#/////////| E-mail: christopher.lindblom@spray.se			  |/////////#
#/////////| Year: 2002										  |/////////#
#/////////| Comment: I'm sorry for my swedish comments.       |/////////#
#/////////| Feel free to change and use this code as much as  |/////////#
#/////////| you please. If you have any questions, just drop  |/////////#
#/////////| a mail in my mail box and I'll try to help U.     |/////////#
#/////////|---------------------------------------------------|/////////#
#///////////////////////////////////////////////////////////////////////#
#######################################################################*/

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>

void server(); // Server delens huvuddel.
void client(); // Client delens huvuddel.
void clear(); // Rensar spelbrädan.
void paint(); // Ritar ut spelbrädan.
void inputter(); // Tar emot inmatning.
void move(); // Flyttar spelbrickorna på spelbrädan.
void quitter(); // Sätter spelloopen till noll.
void check_win(); // Ser efter om man har fått en rad än.
void wait_turn(); // Väntar tills min tur.
void init_server(); // Allt server start skit.
void init_client(); // Allt client start skit.
void move_opponent(); // Flyttar motspelarens gubbar.
void change_player(); // Byter spelare.
void winner_sign();

ifstream read_file; // Read from file-strömmen.
ofstream write_file; // Write to file-strömmen.
char address[5]; // Filnamnet på filen som hinnehåller speldatan.
char player[2]; // Vem som är vem, 0=Spelare 1=Motståndare.
char winner[1]; // Vinnaren.
char board[3][3]; // Vad som är plaserat var på brädan.
char command[5]; // Vilken man vill flytta och vart +vems tur det är.
int command_num[4]; // Kommand fast i intar.
int varv; // Hur många gånger du har mixtrat med dina gubbar.
int rotate; // Ser efter om man ska rotera eller inte.
int counter_move; // En räknare.
int playing; // Spelloopen. 1=Spela 0=Avsluta

int main()
{
		cout<<"+--------------------------------------------------+"<<endl;
		cout<<"|        ### #  ##  ###  #   ##  ###  #  ##        |"<<endl;
		cout<<"|    #    #  # #     #  # # #     #  # # #    #    |"<<endl;
		cout<<"|   ###   #  # #     #  ### #     #  # # ##  ###   |"<<endl;
		cout<<"|    #    #  # #     #  # # #     #  # # #    #    |"<<endl;
		cout<<"|         #  #  ##   #  # #  ##   #   #  ##        |"<<endl;
		cout<<"|       #######    LUFFAR--SCHACK    #######       |"<<endl;
		cout<<"|       ##   ##########################   ##       |"<<endl;
		cout<<"+--------------------------------------------------+"<<endl;

	char choise[1]; // Svaret på valet sparas i denna choise.
	varv=0; // Vi har inte gått nått varv än.
	clear();
	address[0]='P';
	address[1]='L';
	address[2]='A';
	address[3]='Y';
	int scq;
	scq=1;
	while(scq==1)
	{
		cout<<"What would you like to be?"<<endl;
		cout<<"[S]erver"<<endl;
		cout<<"[C]lient"<<endl;
		cout<<"[Q]uit"<<endl<<">";
		cin>>choise[0];
		cout<<endl;
		if(choise[0]=='s'||choise[0]=='S')
		{
			scq=0;
			server(); // Startar Tic Tac Toe i Server läge.
		}
		else if(choise[0]=='c'||choise[0]=='C')
		{
			scq=0;
			client(); // Startar TTT i Client läge.
		}
		else if(choise[0]=='q'||choise[0]=='Q')
		{
			scq=0;
		}
	}
	remove("PLAY");
	return 0;
}

void client() // Client delens huvud del.
{
	init_client();
	if(playing==1) // Starten gick bra.
	{
		paint(); // Så att man slipper sitta och titta på en tom skärm.
	}
	while(playing==1)
	{
		wait_turn();
		move_opponent();
		check_win();
		if(winner[0]==player[0]||winner[0]==player[1]) // Om man har vunnit.
		{
			paint();
			quitter();
		}
		else
		{
			inputter();
			check_win();
			if(winner[0]==player[0]||winner[0]==player[1]) // Om man har vunnit.
			{
				paint();
				quitter();
			}
			else
			{
				paint();
			}
			change_player();
		}
		winner_sign();
	}
}
void server() // Server delens huvud del.
{
	init_server(); // Gör alla start grejer.
	while(playing==1)
	{
		inputter(); // Tar emot inmatning, flyttar.
		check_win(); // Ser efter om man har vunnit.
		if(winner[0]==player[0]||winner[0]==player[1]) // Om man har vunnit.
		{
			paint();
			quitter();
		}
		else
		{
			paint();
		}
		change_player(); // Byter spelare.
		if(playing==1) // Om jag har vunnit.
		{
			wait_turn(); // Väntar på att det ska blir jag.
			move_opponent(); // Flyttar motspelaren.
			check_win(); // Ser efter om motspelaren har vunnit
			if(winner[0]==player[0]||winner[0]==player[1]) // Om man har vunnnit.
			{
				paint();
				quitter();
			}
		}
	}
	winner_sign();
}


void clear() // Rensar hela banan.
{
	int clearer[2];
	for(clearer[0]=0;clearer[0]<3;clearer[0]++)
	{
		for(clearer[1]=0;clearer[1]<3;clearer[1]++)
		{
			board[clearer[0]][clearer[1]]=' ';
		}
	}
}

void inputter() // Har hand om inmatning
{
	rotate=1;
	while(rotate==1)
	{
		rotate=0;
		paint();
		if(varv<3) // Kollar om man har placerat ut alla sina brickor.
		{
			counter_move=2;
			cout<<"Where would you like to put your brick?"<<endl<<">XX";
			cin>>command[2]>>command[3];
			command[0]='0';
			command[1]='0';
		}
		else
		{
			counter_move=0;
			cout<<"Which one and where would you like to move? ([Q]uit)"<<endl<<">";
			cin>>command;
		}
		if(command[0]=='q'||command[0]=='Q') // Ser efter om man har skrivit q.
		{
			quitter(); // Avlutar spellopen.
			rotate=2; // Bara för att han inte ska rammla in i move().
		}
		else
		{
			for(;counter_move<4;counter_move++) // Gör om så att det skrivs in rätt siffror i command_num.
			{
				if(command[counter_move]=='1')
				{
					command_num[counter_move]=0;
				}
				else if(command[counter_move]=='2')
				{
					command_num[counter_move]=1;
				}
				else if(command[counter_move]=='3')
				{
					command_num[counter_move]=2;
				}
				else // Fel aktig inmatning gå ett varv till.
				{
					rotate=1;
				}
			}
		}
		if(rotate==0) // Inmatning korrekt gjord, flytta spelarna.
		{
			move();
		}
	}
	varv++;
}

void move() // Flyttar om brickorna på spelplanen.
{
	if(varv<3) // Om man har gått mindre än 3 varv.
	{
		if(!(board[command_num[2]][command_num[3]]==' '))
		{
			rotate=1;
		}
		else
		{
			board[command_num[2]][command_num[3]]=player[0];
		}
	}
	else
	{
		if(!(board[command_num[0]][command_num[1]]==player[0]))
		{
			rotate=1;
		}
		else if(!(board[command_num[2]][command_num[3]]==' '))
		{
			rotate=1;
		}
		else
		{
			board[command_num[0]][command_num[1]]=' ';
			board[command_num[2]][command_num[3]]=player[0];
		}
	}
}

void quitter() // Sätter spelloopen till noll.
{
	playing=0;
}

void paint() // Ritar ut brädan
{
	cout<<endl<<"      - The board -"<<endl;
	cout<<"      =====["<<player[0]<<"]====="<<endl;
	cout<<"\t  1 2 3 "<<endl;
	cout<<"\t -------"<<endl;
	cout<<"\t1|"<<board[0][0]<<"|"<<board[0][1]<<"|"<<board[0][2]<<"|"<<endl;
	cout<<"\t -------"<<endl;
	cout<<"\t2|"<<board[1][0]<<"|"<<board[1][1]<<"|"<<board[1][2]<<"|"<<endl;
	cout<<"\t -------"<<endl;
	cout<<"\t3|"<<board[2][0]<<"|"<<board[2][1]<<"|"<<board[2][2]<<"|"<<endl;
	cout<<"\t -------"<<endl;
}

void check_win() // Ser efter om man har fått en rad än.
{
	for(counter_move=0;counter_move<2;counter_move++)
	{
		if(board[0][0]==player[counter_move]&&board[0][1]==player[counter_move]&&board[0][2]==player[counter_move])
		{
			winner[0]=player[counter_move];
		}
		else if(board[1][0]==player[counter_move]&&board[1][1]==player[counter_move]&&board[1][2]==player[counter_move])
		{
			winner[0]=player[counter_move];
		}
		else if(board[2][0]==player[counter_move]&&board[2][1]==player[counter_move]&&board[2][2]==player[counter_move])
		{
			winner[0]=player[counter_move];
		}
		else if(board[0][0]==player[counter_move]&&board[1][1]==player[counter_move]&&board[2][2]==player[counter_move])
		{
			winner[0]=player[counter_move];
		}
		else if(board[0][0]==player[counter_move]&&board[1][0]==player[counter_move]&&board[2][0]==player[counter_move])
		{
			winner[0]=player[counter_move];
		}
		else if(board[0][1]==player[counter_move]&&board[1][1]==player[counter_move]&&board[2][1]==player[counter_move])
		{
			winner[0]=player[counter_move];
		}
		else if(board[0][2]==player[counter_move]&&board[1][2]==player[counter_move]&&board[2][2]==player[counter_move])
		{
			winner[0]=player[counter_move];
		}
		else if(board[0][2]==player[counter_move]&&board[1][1]==player[counter_move]&&board[2][0]==player[counter_move])
		{
			winner[0]=player[counter_move];
		}
	}
}

void wait_turn() // Väntar tills det blir min tur.
{
	counter_move=0;
	cout<<"Wait.."<<endl;
	while(counter_move==0)
	{
		read_file.open(address);
		read_file>>command;
		read_file.close();
		if(command[4]==player[0])
		{		
			counter_move=1;
		}
	}
}

void init_server() // Start grejerna.
{
	playing=1; // Sätter spelloopen till ett.
	while(!(player[0]=='X'||player[0]=='O'||player[0]=='x'||player[0]=='o')) // Val av spelare
	{
	cout<<"Which character would you like to be?"<<endl;
	cout<<"[X]"<<endl;
	cout<<"[O]"<<endl<<">";
	cin>>player[0];
	}
	cout<<"Waiting for other player to connect."<<endl;
	if(player[0]=='x') // Storbokstav - X
	{
		player[0]='X';
		player[1]='O';
	}
	if(player[0]=='o') // Storbokstav - O
	{
		player[0]='O';
		player[1]='X';
	}
	write_file.open(address);
	write_file<<"0000"<<player[1];
	write_file.close();
	counter_move=1;
	while(counter_move==1) // Väntar på att motståndare ska joina.
	{
		read_file.open(address);
		read_file>>command;
		read_file.close();
		if(command[4]==player[0])
		{
			counter_move=0;
		}
	}
	cout<<endl;
}

void move_opponent() // Flyttar motståndarens gubbar.
{
	if(command[0]=='0'&&command[1]=='0')
	{
		counter_move=2;
		command_num[0]=4;
	}
	else
	{
		counter_move=0;
	}
	if(!(counter_move==3)) // Om han inte har vunnit så matas data in i command_num.
	{
		for(;counter_move<4;counter_move++) // Gör om så att det skrivs in rätt siffror i command_num.
		{
			if(command[counter_move]=='1')
			{
				command_num[counter_move]=0;
			}
			else if(command[counter_move]=='2')
			{
				command_num[counter_move]=1;
			}
			else if(command[counter_move]=='3')
			{
				command_num[counter_move]=2;
			}
		}
		if(command_num[0]==4) // Placerar ut ny gubbe.
		{
			board[command_num[2]][command_num[3]]=player[1];
		}
		else // Flyttar gubbe.
		{
			board[command_num[0]][command_num[1]]=' ';
			board[command_num[2]][command_num[3]]=player[1];
		}
	}
	check_win();
}

void init_client() // Allt som har med uppstarten av clienten.
{
	read_file.open(address);
	read_file>>command;
	read_file.close();
	if(command[0]=='0'&&command[1]=='0'&&command[2]=='0'&&command[3]=='0')
	{
		playing=1;
		player[0]=command[4];
		if(player[0]=='O')
		{
			player[1]='X';
		}
		else
		{
			player[1]='O';
		}
		write_file.open(address);
		write_file<<"0000"<<player[1];
		write_file.close();
	}
	else
	{
		cout<<"Unable to connect to server..\a"<<endl;
		playing=0;
	}
}

void change_player()
{
	command[4]=player[1];
	write_file.open(address);
	write_file<<command;
	write_file.close();
}

void winner_sign()
{
	char winner_char[1];
	if(winner[0]==player[0]) // Skriver ut text om nån förlorat.
	{
		cout<<"+--------------------------------------------------+"<<endl;
		cout<<"|  ##    ## ## ###   ## ###   ## #####  ####   ##  |"<<endl;
		cout<<"|  ##    ##    ####  ## ####  ## #     ##  ##  ##  |"<<endl;
		cout<<"|  ##    ## ## ## ## ## ## ## ## ###   #####   ##  |"<<endl;
		cout<<"|  ## ## ## ## ##  #### ##  #### #     ###     ##  |"<<endl;
		cout<<"|  ###  ### ## ##   ### ##   ### ##### ####        |"<<endl;
		cout<<"|  ##    ## ## ##    ## ##    ##       ## ##   ##  |"<<endl;
		cout<<"+--------------------------------------------------+"<<endl;
		cout<<"Type something and press [ENTER] to end game..";
		cin>>winner_char[0];
	}
	else if(winner[0]==player[1])
	{
		cout<<"+--------------------------------------------------+"<<endl;
		cout<<"|  ###      ####   ####         #######  ####  ##  |"<<endl;
		cout<<"|  ###     ##  ## ##  ##  ####  ##      ##  ## ##  |"<<endl;
		cout<<"|  ###     ##  ## ##  ## ##     ####    #####  ##  |"<<endl;
		cout<<"|  ###     ##  ## ##  ##  ####  ####    ###    ##  |"<<endl;
		cout<<"|  ####### ##  ## ##  ##     ## ##      ####       |"<<endl;
		cout<<"|  #######  ####   ####   ####  ####### ## ##  ##  |"<<endl;
		cout<<"+--------------------------------------------------+"<<endl;
		cout<<"Type something and press [ENTER] to end game..";
		cin>>winner_char[0];
	}
}