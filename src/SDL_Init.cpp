#include "SdlHeader.h"

int initialization()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
    {
        cout << "Ошибки инициализации SDL: " << SDL_GetError();
        return 1;
    }
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048);
    return 0;
}

void GetInfoFromFile(int *WIDTH, int *HEIGHT, int *VOLUME, int *FULL_SCREEN, int *MENU_AMBIENT)
{
    string line;
    int DATA[4];
    string DATADocs[4] = { " ;  Разрешение экрана"," ;   Вкл/Выкл музыка в меню", " ;   Полный экран или нет", " ;  Громкость звука"};
    int i = 0;
    ifstream in("./gamedata/configs/Menu_cfg.txt"); // окрываем файл для чтения
    if (in.is_open())
    {
        while (getline(in, line))
        {
            DATA[i] = stoi(line);
            i++;
            line.clear();
        }
    }
    in.close();     // закрываем файл
    
    *MENU_AMBIENT = DATA[1];
    *FULL_SCREEN = DATA[2];
    *VOLUME = DATA[3];

    switch (DATA[0])
    {
    case 0:
        *WIDTH = 1920;
        *HEIGHT = 1080;
        break;
    case 1:
        *WIDTH = 1600;
        *HEIGHT = 900;
        break;
    case 2:
        *WIDTH = 1366;
        *HEIGHT = 768;
        break;
    case 3: default:
        *WIDTH = 1024;
        *HEIGHT = 768;
        break;
    }

}

void SaveInfoInFile(int WIDTH, int VOLUME, int FULL_SCREEN, int MENU_AMBIENT)
{
    int Size = 0;
    switch (WIDTH)
    {
    case 1920:
        Size = 0;
        break;
    case 1600:
        Size = 1;
        break;
    case 1366:
        Size = 2;
        break;
    case 1024:
        Size = 3;
        break;
    }
    int DATA[4] = { Size, MENU_AMBIENT, FULL_SCREEN, VOLUME};
    string DATADocs[4] = { " ;  Разрешение экрана"," ;   Вкл/Выкл музыка в меню", " ;   Полный экран или нет", " ;  Громкость звука" };
    int i = 0;
    ofstream out;          // поток для записи
    out.open("./gamedata/configs/Menu_cfg.txt"); // окрываем файл для записи
    if (out.is_open())
    {
        while (i < 4)
        {
            out << DATA[i] << DATADocs[i] << endl;
            i++;
        }
    }
}

void InitMenuTexturesPositions(SDL_Rect* MenuBlocksScreenPos, float Width, float Height)
{
	// Выход
	MenuBlocksScreenPos[0].w = 590 * (Width / 1920);
	MenuBlocksScreenPos[0].h = 182 * (Height / 1080);
	MenuBlocksScreenPos[0].x = Width / 1.5 - MenuBlocksScreenPos[0].w;
	MenuBlocksScreenPos[0].y = Height - MenuBlocksScreenPos[0].h - 25;

	// Настройки
	MenuBlocksScreenPos[1].w = 590 * (Width / 1920);
	MenuBlocksScreenPos[1].h = 182 * (Height / 1080);
	MenuBlocksScreenPos[1].x = MenuBlocksScreenPos[0].x;
	MenuBlocksScreenPos[1].y = MenuBlocksScreenPos[0].y - MenuBlocksScreenPos[1].h - 10;

	// Играть
	MenuBlocksScreenPos[2].w = 590 * (Width / 1920);
	MenuBlocksScreenPos[2].h = 185 * (Height / 1080);
	MenuBlocksScreenPos[2].x = MenuBlocksScreenPos[0].x;
	MenuBlocksScreenPos[2].y = MenuBlocksScreenPos[1].y - MenuBlocksScreenPos[2].h - 10;

	// Управление
	MenuBlocksScreenPos[3].w = 590 * (Width / 1920);
	MenuBlocksScreenPos[3].h = 182 * (Height / 1080);
	MenuBlocksScreenPos[3].x = MenuBlocksScreenPos[0].x;
	MenuBlocksScreenPos[3].y = MenuBlocksScreenPos[2].y - MenuBlocksScreenPos[3].h - 10;

	// Разрешение экрана
	MenuBlocksScreenPos[4].w = 644 * (Width / 1920);
	MenuBlocksScreenPos[4].h = 334 * (Height / 1080);
	MenuBlocksScreenPos[4].x = Width - MenuBlocksScreenPos[4].w - 45;
	MenuBlocksScreenPos[4].y = Height / 6;
    
	// громкость
	MenuBlocksScreenPos[5].w = 644 * (Width / 1920);
	MenuBlocksScreenPos[5].h = 201 * (Height / 1080);
	MenuBlocksScreenPos[5].x = MenuBlocksScreenPos[4].x;
	MenuBlocksScreenPos[5].y = MenuBlocksScreenPos[4].y + MenuBlocksScreenPos[4].h + 5;

	// вкл/выкл музыку в меню
	MenuBlocksScreenPos[6].w = 562 * (Width / 1920);
	MenuBlocksScreenPos[6].h = 70 * (Height / 1080);
	MenuBlocksScreenPos[6].x = MenuBlocksScreenPos[4].x;
	MenuBlocksScreenPos[6].y = MenuBlocksScreenPos[5].y + MenuBlocksScreenPos[5].h + 5;

    // Полноэкранный режим
    MenuBlocksScreenPos[7].w = 562 * (Width / 1920);
    MenuBlocksScreenPos[7].h = 70 * (Height / 1080);
    MenuBlocksScreenPos[7].x = MenuBlocksScreenPos[6].x;
    MenuBlocksScreenPos[7].y = MenuBlocksScreenPos[6].y + MenuBlocksScreenPos[6].h + 5;
}

void InitMenuAtlasPositions(SDL_Rect* MenuBlocks)
{
	// Выход
	MenuBlocks[0].x = 3;
	MenuBlocks[0].y = 567;
	MenuBlocks[0].w = 590;
	MenuBlocks[0].h = 182;

	// Настройки
	MenuBlocks[1].x = 3;
	MenuBlocks[1].y = 377;
	MenuBlocks[1].w = 590;
	MenuBlocks[1].h = 182;

	// Играть
	MenuBlocks[2].x = 3;
	MenuBlocks[2].y = 189;
	MenuBlocks[2].w = 590;
	MenuBlocks[2].h = 185;

	// Управление
	MenuBlocks[3].x = 3;
	MenuBlocks[3].y = 2;
	MenuBlocks[3].w = 590;
	MenuBlocks[3].h = 182;

	// Разрешение экрана
	MenuBlocks[4].x = 637;
	MenuBlocks[4].y = 55;
	MenuBlocks[4].w = 644;
	MenuBlocks[4].h = 334;

	// Уровень громкости
	MenuBlocks[5].x = 637;
	MenuBlocks[5].y = 441;
	MenuBlocks[5].w = 644;
	MenuBlocks[5].h = 201;

	// Музыка в меню
	MenuBlocks[6].x = 637;
	MenuBlocks[6].y = 680;
	MenuBlocks[6].w = 562;
	MenuBlocks[6].h = 70;

	// Ползунок уровня громкости
	MenuBlocks[7].x = 1301;
	MenuBlocks[7].y = 518;
	MenuBlocks[7].w = 24;
	MenuBlocks[7].h = 91;

	// Крестик для вкл/выкл музыки
	MenuBlocks[8].x = 1218;
	MenuBlocks[8].y = 692;
	MenuBlocks[8].w = 46;
	MenuBlocks[8].h = 46;

	// Выделение пункта меню
	MenuBlocks[9].x = 1;
	MenuBlocks[9].y = 750;
	MenuBlocks[9].w = 598;
	MenuBlocks[9].h = 191;

    // Полноэкранный режим
    MenuBlocks[10].x = 637;
    MenuBlocks[10].y = 752;
    MenuBlocks[10].w = 562;
    MenuBlocks[10].h = 70;
}

void InitPlayTexturesPositions(SDL_Rect* PlayBlocksScreenPos, float Width, float Height)
{
    //Аккорды
    PlayBlocksScreenPos[0].x = 15;
    PlayBlocksScreenPos[0].y = 15;
    PlayBlocksScreenPos[0].w = 737 * (Width / 1920);
    PlayBlocksScreenPos[0].h = 344 * (Height / 1080);

    //ГИТАРА
    PlayBlocksScreenPos[1].w = 1920 * (Width / 1920);
    PlayBlocksScreenPos[1].h = 391 * (Height / 1080);
    PlayBlocksScreenPos[1].x = 0;
    PlayBlocksScreenPos[1].y = Height - PlayBlocksScreenPos[1].h;

    //Кнопка аккорды
    PlayBlocksScreenPos[2].w = 162 * (Width / 1920);
    PlayBlocksScreenPos[2].h = 64 * (Height / 1080);
    PlayBlocksScreenPos[2].x = Width - PlayBlocksScreenPos[2].w;
    PlayBlocksScreenPos[2].y = PlayBlocksScreenPos[1].y - 10 - PlayBlocksScreenPos[2].h;

    //Кнопка струны
    PlayBlocksScreenPos[3].w = 162 * (Width / 1920);
    PlayBlocksScreenPos[3].h = 64 * (Height / 1080);
    PlayBlocksScreenPos[3].x = PlayBlocksScreenPos[2].x - 10 - PlayBlocksScreenPos[3].w;
    PlayBlocksScreenPos[3].y = PlayBlocksScreenPos[1].y - 10 - PlayBlocksScreenPos[3].h;

    //Кнопка перебор
    PlayBlocksScreenPos[4].w = 162 * (Width / 1920);
    PlayBlocksScreenPos[4].h = 64 * (Height / 1080);
    PlayBlocksScreenPos[4].x = PlayBlocksScreenPos[3].x - 10 - PlayBlocksScreenPos[4].w;
    PlayBlocksScreenPos[4].y = PlayBlocksScreenPos[1].y - 10 - PlayBlocksScreenPos[4].h;

    //ВЫХОД
    PlayBlocksScreenPos[5].w = 187 * (Width / 1920);
    PlayBlocksScreenPos[5].h = 56 * (Height / 1080);
    PlayBlocksScreenPos[5].x = Width - PlayBlocksScreenPos[5].w;
    PlayBlocksScreenPos[5].y = 0;

    //ТОНАЛЬНОСТЬ
    PlayBlocksScreenPos[6].w = 337 * (Width / 1920);
    PlayBlocksScreenPos[6].h = 65 * (Height / 1080);
    PlayBlocksScreenPos[6].x = 15;
    PlayBlocksScreenPos[6].y = PlayBlocksScreenPos[1].y - 10 - PlayBlocksScreenPos[6].h;

    //БОЙ
    PlayBlocksScreenPos[7].w = 185 * (Width / 1920);
    PlayBlocksScreenPos[7].h = 65 * (Height / 1080);
    PlayBlocksScreenPos[7].x = PlayBlocksScreenPos[6].w + 25;
    PlayBlocksScreenPos[7].y = PlayBlocksScreenPos[1].y - 10 - PlayBlocksScreenPos[7].h;

    //Показатель тональности
    PlayBlocksScreenPos[8].w = 19 * (Width / 1920);
    PlayBlocksScreenPos[8].h = 23 * (Height / 1080);
    PlayBlocksScreenPos[8].x = 5 + PlayBlocksScreenPos[6].w / 2;
    PlayBlocksScreenPos[8].y = PlayBlocksScreenPos[6].y + 33 * (Height / 1080);

    //Закрывалка для 7 аккордов
    PlayBlocksScreenPos[9].x = 16;
    PlayBlocksScreenPos[9].y = 260 * (Height / 1080);
    PlayBlocksScreenPos[9].w = 730 * (Width / 1920);
    PlayBlocksScreenPos[9].h = 100 * (Height / 1080);

    //БОЙ -68
    PlayBlocksScreenPos[10].w = 20 * (Width / 1920);
    PlayBlocksScreenPos[10].h = 35 * (Height / 1080);
    PlayBlocksScreenPos[10].x = PlayBlocksScreenPos[7].x + 140 * (Width / 1920);
    PlayBlocksScreenPos[10].y = PlayBlocksScreenPos[7].y + 15 * (Height / 1080);

    //БОЙ *
    PlayBlocksScreenPos[11].w = 14 * (Width / 1920);
    PlayBlocksScreenPos[11].h = 15 * (Height / 1080);
    PlayBlocksScreenPos[11].x = PlayBlocksScreenPos[10].x - 16 * (Width / 1920);
    PlayBlocksScreenPos[11].y = PlayBlocksScreenPos[10].y + 4;
}

void InitSettingAtlasPositions(SDL_Rect* Settings)
{
    // Выход из настроек/управления
    Settings[0].x = 625;
    Settings[0].y = 0;
    Settings[0].w = 183;
    Settings[0].h = 52;

    // Разрешение

    // 1920x1080
    Settings[1].x = 824;
    Settings[1].y = 150;
    Settings[1].w = 274;
    Settings[1].h = 47;

    // 1600x900
    Settings[2].x = 840;
    Settings[2].y = 210;
    Settings[2].w = 249;
    Settings[2].h = 47;

    // 1366x768
    Settings[3].x = 839;
    Settings[3].y = 267;
    Settings[3].w = 249;
    Settings[3].h = 47;

    // 1024x768
    Settings[4].x = 837;
    Settings[4].y = 333;
    Settings[4].w = 252;
    Settings[4].h = 47;

    // вкл/выкл музыку в меню
    Settings[5].x = 1219;
    Settings[5].y = 693;
    Settings[5].w = 44;
    Settings[5].h = 44;

    // Элипс для показа громкости
    Settings[6].x = 1301;
    Settings[6].y = 519;
    Settings[6].w = 24;
    Settings[6].h = 89;
}

void InitSettingTexturesPositions(SDL_Rect* SettingsScreenPos, SDL_Rect* atlasScreen,float Width, float Height)
{
    // Выход из настроек/управления
    SettingsScreenPos[0].w = 183 * (Width / 1920);
    SettingsScreenPos[0].h = 52 * (Height / 1080);
    SettingsScreenPos[0].x = Width - SettingsScreenPos[0].w;
    SettingsScreenPos[0].y = 1;

    // 1920x1080
    SettingsScreenPos[1].w = 274 * (Width / 1920);
    SettingsScreenPos[1].h = 47 * (Height / 1080);
    SettingsScreenPos[1].x = atlasScreen[4].x + atlasScreen[4].w / 3.5;
    SettingsScreenPos[1].y = atlasScreen[4].y + 95 * (Height / 1080);

    // 1600x900
    SettingsScreenPos[2].w = 249 * (Width / 1920);
    SettingsScreenPos[2].h = 47 * (Height / 1080);
    SettingsScreenPos[2].x = SettingsScreenPos[1].x + 24 * (Width / 1920);
    SettingsScreenPos[2].y = SettingsScreenPos[1].y + SettingsScreenPos[1].h + 10 * (Height / 1080);

    // 1366x768
    SettingsScreenPos[3].w = 249 * (Width / 1920);
    SettingsScreenPos[3].h = 47 * (Height / 1080);
    SettingsScreenPos[3].x = SettingsScreenPos[2].x;
    SettingsScreenPos[3].y = SettingsScreenPos[2].y + SettingsScreenPos[2].h + 15 * (Height / 1080);

    // 1024x768
    SettingsScreenPos[4].w = 249 * (Width / 1920);
    SettingsScreenPos[4].h = 47 * (Height / 1080);
    SettingsScreenPos[4].x = SettingsScreenPos[1].x + 20 * (Width / 1920);
    SettingsScreenPos[4].y = SettingsScreenPos[3].y + SettingsScreenPos[3].h + 20 * (Height / 1080);

    // Регулировка громкости
    SettingsScreenPos[5].w = 575 * (Width / 1920);
    SettingsScreenPos[5].h = 30 * (Height / 1080);
    SettingsScreenPos[5].x = atlasScreen[5].x + 40 * (Width / 1920);
    SettingsScreenPos[5].y = atlasScreen[5].y + 110 * (Height / 1080);

    //Элипс громкости
    SettingsScreenPos[6].w = 24 * (Width / 1920);
    SettingsScreenPos[6].h = 89 * (Height / 1080);
    SettingsScreenPos[6].x = SettingsScreenPos[5].x + SettingsScreenPos[5].w;
    SettingsScreenPos[6].y = SettingsScreenPos[5].y - SettingsScreenPos[6].h / 3;

    // Квадрат вкл/выкл музыку
    SettingsScreenPos[7].w = 44 * (Width / 1920);
    SettingsScreenPos[7].h = 44 * (Height / 1080);
    SettingsScreenPos[7].x = SettingsScreenPos[1].x + SettingsScreenPos[1].w + 34 * (Width / 1920);
    SettingsScreenPos[7].y = SettingsScreenPos[6].y + 137 * (Height / 1080);

    // Квадрат полный экран/окно
    SettingsScreenPos[8].w = 44 * (Width / 1920);
    SettingsScreenPos[8].h = 44 * (Height / 1080);
    SettingsScreenPos[8].x = SettingsScreenPos[7].x;
    SettingsScreenPos[8].y = SettingsScreenPos[6].y + 213 * (Height / 1080);
}

void InitPlayAtlasPositions(SDL_Rect* PlayBlocks)
{
    //Аккорды
    PlayBlocks[0].x = 0;
    PlayBlocks[0].y = 4;
    PlayBlocks[0].w = 737;
    PlayBlocks[0].h = 344;

    //Гитара
    PlayBlocks[1].x = 370;
    PlayBlocks[1].y = 553;
    PlayBlocks[1].w = 1535;
    PlayBlocks[1].h = 391;

    //Кнопка аккорды
    PlayBlocks[2].x = 342;
    PlayBlocks[2].y = 486;
    PlayBlocks[2].w = 162;
    PlayBlocks[2].h = 64;

    //Кнопка струны
    PlayBlocks[3].x = 504;
    PlayBlocks[3].y = 486;
    PlayBlocks[3].w = 162;
    PlayBlocks[3].h = 63;

    //Кнопка перебор
    PlayBlocks[4].x = 9;
    PlayBlocks[4].y = 487;
    PlayBlocks[4].w = 162;
    PlayBlocks[4].h = 64;

    //ВЫХОД
    PlayBlocks[5].x = 10;
    PlayBlocks[5].y = 777;
    PlayBlocks[5].w = 187;
    PlayBlocks[5].h = 56;

    //Тональность
    PlayBlocks[6].x = 8;
    PlayBlocks[6].y = 571;
    PlayBlocks[6].w = 334;
    PlayBlocks[6].h = 63;

    //Бой
    PlayBlocks[7].x = 11;
    PlayBlocks[7].y = 716;
    PlayBlocks[7].w = 185;
    PlayBlocks[7].h = 54;
    
    //Выделение аккорда
    PlayBlocks[8].x = 848;
    PlayBlocks[8].y = 121;
    PlayBlocks[8].w = 105;
    PlayBlocks[8].h = 105;

    //Выделение струны/аккорды
    PlayBlocks[9].x = 172;
    PlayBlocks[9].y = 483;
    PlayBlocks[9].w = 169;
    PlayBlocks[9].h = 67;

    //Круг для показа текущей ноты
    PlayBlocks[10].x = 676;
    PlayBlocks[10].y = 489;
    PlayBlocks[10].w = 59;
    PlayBlocks[10].h = 59;

    //Эллипс для показа текущей тональности
    PlayBlocks[11].x = 347;
    PlayBlocks[11].y = 604;
    PlayBlocks[11].w = 19;
    PlayBlocks[11].h = 23;

    //Закрывалка для 7 аккордов
    PlayBlocks[12].x = 7;
    PlayBlocks[12].y = 366;
    PlayBlocks[12].w = 730;
    PlayBlocks[12].h = 100;

    //Бой -
    PlayBlocks[13].x = 209;
    PlayBlocks[13].y = 728;
    PlayBlocks[13].w = 20;
    PlayBlocks[13].h = 35;

    //Бой 6
    PlayBlocks[14].x = 300;
    PlayBlocks[14].y = 725;
    PlayBlocks[14].w = 20;
    PlayBlocks[14].h = 35;

    //Бой 8
    PlayBlocks[15].x = 344;
    PlayBlocks[15].y = 724;
    PlayBlocks[15].w = 17;
    PlayBlocks[15].h = 36;

    //Бой *
    PlayBlocks[16].x = 262;
    PlayBlocks[16].y = 739;
    PlayBlocks[16].w = 14;
    PlayBlocks[16].h = 15;
}
