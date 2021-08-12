#ifndef SAVELOAD_H
#define SAVELOAD_H

/** @defgroup saveload Save and Load

	Functions to use the save and load screen
	@{
*/

/**Save and Load cancel*/
#define OSL_SAVELOAD_CANCEL 1
/**Save and Load OK*/
#define OSL_SAVELOAD_OK 0

/**Save dialog*/
#define OSL_DIALOG_SAVE 1
/**Load dialog*/
#define OSL_DIALOG_LOAD 2

/**Struct containing data to save/load
	\param *gameTitle
        Title of the fame
	\param *gameID
        Game ID of the fame
	\param *saveName
        Save name
	\param *nameList
        List of names for saves data
	\param *pic1
        Pic1 data
	\param *size_pic1
        Pic1 size
	\param *icon0
        Icon0 data
	\param *size_icon0
        Icon0 size
	\param *data
        Pointer to data to be saved/loaded
	\param dataSize
        Size of data to be saved/loaded
*/
struct oslSaveLoad{
    char gameTitle[0x80];
    char gameID[13];
    char saveName[20];
    char (*nameList)[20];
    unsigned char *pic1;
    unsigned int size_pic1;
    unsigned char *icon0;
    unsigned int size_icon0;
    void *data;
    int dataSize;
};

/**Initializes the save dialog
	\param *saveData
        Pointer to a oslSaveLoad structure containing data to be saved
*/
void oslInitSaveDialog(struct oslSaveLoad *saveData);


/**Initializes the load dialog
	\param *saveData
        Pointer to a oslSaveLoad structure where data will be loaded
*/
void oslInitLoadDialog(struct oslSaveLoad *loadData);

/**Draws the save/load dialog
*/
void oslDrawSaveLoad();

/**Returns the current dialog status
*/
int oslGetLoadSaveStatus();

/**Returns the current dialog type
*/
int oslGetSaveLoadType();

/**Gets the dialog's result
*/
int oslSaveLoadGetResult();

/**Ends the current dialog
*/
void oslEndSaveLoadDialog();

/** @} */ // end of saveload
#endif
