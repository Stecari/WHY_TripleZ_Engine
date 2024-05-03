#pragma once

/*--------------------------Form glfw3.h---------------------------------------------------------*/


/* Printable keys */
#define TZ_KEY_SPACE              32
#define TZ_KEY_APOSTROPHE         39  /* ' */
#define TZ_KEY_COMMA              44  /* , */
#define TZ_KEY_MINUS              45  /* - */
#define TZ_KEY_PERIOD             46  /* . */
#define TZ_KEY_SLASH              47  /* / */
#define TZ_KEY_0                  48
#define TZ_KEY_1                  49
#define TZ_KEY_2                  50
#define TZ_KEY_3                  51
#define TZ_KEY_4                  52
#define TZ_KEY_5                  53
#define TZ_KEY_6                  54
#define TZ_KEY_7                  55
#define TZ_KEY_8                  56
#define TZ_KEY_9                  57
#define TZ_KEY_SEMICOLON          59  /* ; */
#define TZ_KEY_EQUAL              61  /* = */
#define TZ_KEY_A                  65
#define TZ_KEY_B                  66
#define TZ_KEY_C                  67
#define TZ_KEY_D                  68
#define TZ_KEY_E                  69
#define TZ_KEY_F                  70
#define TZ_KEY_G                  71
#define TZ_KEY_H                  72
#define TZ_KEY_I                  73
#define TZ_KEY_J                  74
#define TZ_KEY_K                  75
#define TZ_KEY_L                  76
#define TZ_KEY_M                  77
#define TZ_KEY_N                  78
#define TZ_KEY_O                  79
#define TZ_KEY_P                  80
#define TZ_KEY_Q                  81
#define TZ_KEY_R                  82
#define TZ_KEY_S                  83
#define TZ_KEY_T                  84
#define TZ_KEY_U                  85
#define TZ_KEY_V                  86
#define TZ_KEY_W                  87
#define TZ_KEY_X                  88
#define TZ_KEY_Y                  89
#define TZ_KEY_Z                  90
#define TZ_KEY_LEFT_BRACKET       91  /* [ */
#define TZ_KEY_BACKSLASH          92  /* \ */
#define TZ_KEY_RIGHT_BRACKET      93  /* ] */
#define TZ_KEY_GRAVE_ACCENT       96  /* ` */
#define TZ_KEY_WORLD_1            161 /* non-US #1 */
#define TZ_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define TZ_KEY_ESCAPE             256
#define TZ_KEY_ENTER              257
#define TZ_KEY_TAB                258
#define TZ_KEY_BACKSPACE          259
#define TZ_KEY_INSERT             260
#define TZ_KEY_DELETE             261
#define TZ_KEY_RIGHT              262
#define TZ_KEY_LEFT               263
#define TZ_KEY_DOWN               264
#define TZ_KEY_UP                 265
#define TZ_KEY_PAGE_UP            266
#define TZ_KEY_PAGE_DOWN          267
#define TZ_KEY_HOME               268
#define TZ_KEY_END                269
#define TZ_KEY_CAPS_LOCK          280
#define TZ_KEY_SCROLL_LOCK        281
#define TZ_KEY_NUM_LOCK           282
#define TZ_KEY_PRINT_SCREEN       283
#define TZ_KEY_PAUSE              284
#define TZ_KEY_F1                 290
#define TZ_KEY_F2                 291
#define TZ_KEY_F3                 292
#define TZ_KEY_F4                 293
#define TZ_KEY_F5                 294
#define TZ_KEY_F6                 295
#define TZ_KEY_F7                 296
#define TZ_KEY_F8                 297
#define TZ_KEY_F9                 298
#define TZ_KEY_F10                299
#define TZ_KEY_F11                300
#define TZ_KEY_F12                301
#define TZ_KEY_F13                302
#define TZ_KEY_F14                303
#define TZ_KEY_F15                304
#define TZ_KEY_F16                305
#define TZ_KEY_F17                306
#define TZ_KEY_F18                307
#define TZ_KEY_F19                308
#define TZ_KEY_F20                309
#define TZ_KEY_F21                310
#define TZ_KEY_F22                311
#define TZ_KEY_F23                312
#define TZ_KEY_F24                313
#define TZ_KEY_F25                314
#define TZ_KEY_KP_0               320
#define TZ_KEY_KP_1               321
#define TZ_KEY_KP_2               322
#define TZ_KEY_KP_3               323
#define TZ_KEY_KP_4               324
#define TZ_KEY_KP_5               325
#define TZ_KEY_KP_6               326
#define TZ_KEY_KP_7               327
#define TZ_KEY_KP_8               328
#define TZ_KEY_KP_9               329
#define TZ_KEY_KP_DECIMAL         330
#define TZ_KEY_KP_DIVIDE          331
#define TZ_KEY_KP_MULTIPLY        332
#define TZ_KEY_KP_SUBTRACT        333
#define TZ_KEY_KP_ADD             334
#define TZ_KEY_KP_ENTER           335
#define TZ_KEY_KP_EQUAL           336
#define TZ_KEY_LEFT_SHIFT         340
#define TZ_KEY_LEFT_CONTROL       341
#define TZ_KEY_LEFT_ALT           342
#define TZ_KEY_LEFT_SUPER         343
#define TZ_KEY_RIGHT_SHIFT        344
#define TZ_KEY_RIGHT_CONTROL      345
#define TZ_KEY_RIGHT_ALT          346
#define TZ_KEY_RIGHT_SUPER        347
#define TZ_KEY_MENU               348

#define TZ_KEY_LAST               TZ_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

 /*! @brief If this bit is set one or more Shift keys were held down.
  *
  *  If this bit is set one or more Shift keys were held down.
  */
#define TZ_MOD_SHIFT           0x0001
  /*! @brief If this bit is set one or more Control keys were held down.
   *
   *  If this bit is set one or more Control keys were held down.
   */
#define TZ_MOD_CONTROL         0x0002
   /*! @brief If this bit is set one or more Alt keys were held down.
	*
	*  If this bit is set one or more Alt keys were held down.
	*/
#define TZ_MOD_ALT             0x0004
	/*! @brief If this bit is set one or more Super keys were held down.
	 *
	 *  If this bit is set one or more Super keys were held down.
	 */
#define TZ_MOD_SUPER           0x0008
	 /*! @brief If this bit is set the Caps Lock key is enabled.
	  *
	  *  If this bit is set the Caps Lock key is enabled and the @ref
	  *  TZ_LOCK_KEY_MODS input mode is set.
	  */
#define TZ_MOD_CAPS_LOCK       0x0010
	  /*! @brief If this bit is set the Num Lock key is enabled.
	   *
	   *  If this bit is set the Num Lock key is enabled and the @ref
	   *  TZ_LOCK_KEY_MODS input mode is set.
	   */
#define TZ_MOD_NUM_LOCK        0x0020
