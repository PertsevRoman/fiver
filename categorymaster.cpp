#include "categorymaster.h"

CategoryMaster::CategoryMaster() {
    voteDefends[-4] = "Сильно негативное";
    voteDefends[-3] = "Средне негативное";
    voteDefends[-2] = "Слегка негативное";
    voteDefends[-1] = "Негативное, близкое к нейтральному";
    voteDefends[0] = "Нейтральное";
    voteDefends[1] = "Положительное, близкое к нейтральному";
    voteDefends[2] = "Слегка положительное";
    voteDefends[3] = "Средне положительное";
    voteDefends[4] = "Сильно положительное";
}
