#include "st7789v2.h"
#include "font.h"

/*---------------- 用户处理区域 Start ----------------*/
#define ST7789V2_DELAY_XMS(XMS) HAL_Delay(XMS)

/**
 * @brief   设置RES引脚电平
 * @param   level : 电平 0低 1高
 * @retval
 **/
void st7789v2_res_set_level(ST7789V2_Instance *instance, uint8_t level)
{
    if (0 == level)
    {
        HAL_GPIO_WritePin(instance->res_port, instance->res_pin, GPIO_PIN_RESET);
    }
    else if (1 == level)
    {
        HAL_GPIO_WritePin(instance->res_port, instance->res_pin, GPIO_PIN_SET);
    }
}

/**
 * @brief   设置DC引脚电平
 * @param   level : 电平 0低 1高
 * @retval
 **/
void st7789v2_dc_set_level(ST7789V2_Instance *instance, uint8_t level)
{
    if (0 == level)
    {
        HAL_GPIO_WritePin(instance->dc_port, instance->dc_pin, GPIO_PIN_RESET);
    }
    else if (1 == level)
    {
        HAL_GPIO_WritePin(instance->dc_port, instance->dc_pin, GPIO_PIN_SET);
    }
}

/**
 * @brief   设置CS引脚电平
 * @param   level : 电平 0低 1高
 * @retval
 **/
void st7789v2_cs_set_level(ST7789V2_Instance *instance, uint8_t level)
{
    if (0 == level)
    {
        HAL_GPIO_WritePin(instance->cs_port, instance->cs_pin, GPIO_PIN_RESET);
    }
    else if (1 == level)
    {
        HAL_GPIO_WritePin(instance->cs_port, instance->cs_pin, GPIO_PIN_SET);
    }
}

/**
 * @brief   设置BLK引脚电平
 * @param   level : 电平 0低 1高
 * @retval
 **/
void st7789v2_blk_set_level(ST7789V2_Instance *instance, uint8_t level)
{
    if (0 == level)
    {
        HAL_GPIO_WritePin(instance->blk_port, instance->blk_pin, GPIO_PIN_RESET);
    }
    else if (1 == level)
    {
        HAL_GPIO_WritePin(instance->blk_port, instance->blk_pin, GPIO_PIN_SET);
    }
}

/**
 * @brief   st7789v2的SPI收发数据
 * @param
 * @retval
 **/
uint8_t st7789v2_read_write_byte(ST7789V2_Instance *instance, uint8_t TxData)
{
    uint8_t RxData;
    HAL_SPI_TransmitReceive(instance->hspi, &TxData, &RxData, 1, HAL_MAX_DELAY);
    return RxData;
}
/*---------------- 用户处理区域  End  ----------------*/

/**
 * @brief   ST7789V2串行数据写入函数
 * @param   dat  要写入的串行数据
 * @retval
 **/
void ST7789V2_Writ_Bus(ST7789V2_Instance *instance, uint8_t dat)
{
    st7789v2_cs_set_level(instance, 0);
    st7789v2_read_write_byte(instance, dat);
    st7789v2_cs_set_level(instance, 1);
}

/**
 * @brief   ST7789V2写入数据
 * @param   dat 写入的数据
 * @retval
 **/
void ST7789V2_WR_DATA8(ST7789V2_Instance *instance, uint8_t dat)
{
    st7789v2_dc_set_level(instance, 1);
    ; // 写数据
    ST7789V2_Writ_Bus(instance, dat);
}

/**
 * @brief   ST7789V2写入数据
 * @param   dat 写入的数据
 * @retval
 **/
void ST7789V2_WR_DATA(ST7789V2_Instance *instance, uint16_t dat)
{
    st7789v2_dc_set_level(instance, 1); // 写数据
    ST7789V2_Writ_Bus(instance, dat >> 8);
    ST7789V2_Writ_Bus(instance, dat);
}
/**
 * @brief   ST7789V2写入命令
 * @param   dat 写入的命令
 * @retval
 **/
void ST7789V2_WR_REG(ST7789V2_Instance *instance, uint8_t dat)
{
    st7789v2_dc_set_level(instance, 0); // 写命令
    ST7789V2_Writ_Bus(instance, dat);
    st7789v2_dc_set_level(instance, 1); // 写数据
}

/**
 * @brief   设置起始和结束地址
 * @param   x1,x2 设置列的起始和结束地址
 *          y1,y2 设置行的起始和结束地址
 * @retval
 **/
void ST7789V2_Address_Set(ST7789V2_Instance *instance, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (USE_HORIZONTAL == 0)
    {
        ST7789V2_WR_REG(instance, 0x2a); // 列地址设置
        ST7789V2_WR_DATA(instance, x1);
        ST7789V2_WR_DATA(instance, x2);
        ST7789V2_WR_REG(instance, 0x2b); // 行地址设置
        ST7789V2_WR_DATA(instance, y1);
        ST7789V2_WR_DATA(instance, y2);
        ST7789V2_WR_REG(instance, 0x2c); // 储存器写
    }
    else if (USE_HORIZONTAL == 1)
    {
        ST7789V2_WR_REG(instance, 0x2a); // 列地址设置
        ST7789V2_WR_DATA(instance, x1);
        ST7789V2_WR_DATA(instance, x2);
        ST7789V2_WR_REG(instance, 0x2b); // 行地址设置
        ST7789V2_WR_DATA(instance, y1);
        ST7789V2_WR_DATA(instance, y2);
        ST7789V2_WR_REG(instance, 0x2c); // 储存器写
    }
    else if (USE_HORIZONTAL == 2)
    {
        ST7789V2_WR_REG(instance, 0x2a); // 列地址设置
        ST7789V2_WR_DATA(instance, x1);
        ST7789V2_WR_DATA(instance, x2);
        ST7789V2_WR_REG(instance, 0x2b); // 行地址设置
        ST7789V2_WR_DATA(instance, y1);
        ST7789V2_WR_DATA(instance, y2);
        ST7789V2_WR_REG(instance, 0x2c); // 储存器写
    }
    else
    {
        ST7789V2_WR_REG(instance, 0x2a); // 列地址设置
        ST7789V2_WR_DATA(instance, x1);
        ST7789V2_WR_DATA(instance, x2);
        ST7789V2_WR_REG(instance, 0x2b); // 行地址设置
        ST7789V2_WR_DATA(instance, y1);
        ST7789V2_WR_DATA(instance, y2);
        ST7789V2_WR_REG(instance, 0x2c); // 储存器写
    }
}

/**
 * @brief   注册ST7789V2实例
 */
ST7789V2_Instance *ST7789V2_Register(ST7789V2_InitTypedef *init)
{
    if(init == NULL)
        return NULL;
    if(init->hspi == NULL || init->blk_port == NULL || init->cs_port == NULL || init->dc_port == NULL || init->res_port == NULL)
        return NULL;

    ST7789V2_Instance *instance = malloc(sizeof(ST7789V2_Instance));
    if(instance == NULL)
        return NULL;

    instance->blk_pin = init->blk_pin;
    instance->blk_port = init->blk_port;
    instance->cs_pin = init->cs_pin;
    instance->cs_port = init->cs_port;
    instance->dc_pin = init->dc_pin;
    instance->dc_port = init->dc_port;
    instance->res_pin = init->res_pin;
    instance->res_port = init->res_port;
    instance->hspi = init->hspi;

    return instance;
}

/**
 * @brief   ST7789V2的初始化
 * @param
 * @retval
 **/
void ST7789V2_Init(ST7789V2_Instance *instance)
{
    st7789v2_blk_set_level(instance, 0); // 关闭背光
    st7789v2_res_set_level(instance, 0); // 复位

    ST7789V2_DELAY_XMS(50);

    st7789v2_res_set_level(instance, 1);

    ST7789V2_DELAY_XMS(50);

    st7789v2_blk_set_level(instance, 1); // 打开背光

    ST7789V2_WR_REG(instance, 0x11);

    ST7789V2_DELAY_XMS(10);

    ST7789V2_WR_REG(instance, 0x36);

    if (USE_HORIZONTAL == 0)
        ST7789V2_WR_DATA8(instance, 0x00);
    else if (USE_HORIZONTAL == 1)
        ST7789V2_WR_DATA8(instance, 0xC0);
    else if (USE_HORIZONTAL == 2)
        ST7789V2_WR_DATA8(instance, 0x70);
    else
        ST7789V2_WR_DATA8(instance, 0xA0);

    ST7789V2_WR_REG(instance, 0x3A);
    ST7789V2_WR_DATA8(instance, 0x05);

    ST7789V2_WR_REG(instance, 0xB2);
    ST7789V2_WR_DATA8(instance, 0x0C);
    ST7789V2_WR_DATA8(instance, 0x0C);
    ST7789V2_WR_DATA8(instance, 0x00);
    ST7789V2_WR_DATA8(instance, 0x33);
    ST7789V2_WR_DATA8(instance, 0x33);

    ST7789V2_WR_REG(instance, 0xB7);
    ST7789V2_WR_DATA8(instance, 0x35);

    ST7789V2_WR_REG(instance, 0xBB);
    ST7789V2_WR_DATA8(instance, 0x35);

    ST7789V2_WR_REG(instance, 0xC0);
    ST7789V2_WR_DATA8(instance, 0x2C);

    ST7789V2_WR_REG(instance, 0xC2);
    ST7789V2_WR_DATA8(instance, 0x01);

    ST7789V2_WR_REG(instance, 0xC3);
    ST7789V2_WR_DATA8(instance, 0x13);

    ST7789V2_WR_REG(instance, 0xC4);
    ST7789V2_WR_DATA8(instance, 0x20);

    ST7789V2_WR_REG(instance, 0xC6);
    ST7789V2_WR_DATA8(instance, 0x0F);

    ST7789V2_WR_REG(instance, 0xD0);
    ST7789V2_WR_DATA8(instance, 0xA4);
    ST7789V2_WR_DATA8(instance, 0xA1);

    ST7789V2_WR_REG(instance, 0xD6);
    ST7789V2_WR_DATA8(instance, 0xA1);

    ST7789V2_WR_REG(instance, 0xE0);
    ST7789V2_WR_DATA8(instance, 0xF0);
    ST7789V2_WR_DATA8(instance, 0x00);
    ST7789V2_WR_DATA8(instance, 0x04);
    ST7789V2_WR_DATA8(instance, 0x04);
    ST7789V2_WR_DATA8(instance, 0x04);
    ST7789V2_WR_DATA8(instance, 0x05);
    ST7789V2_WR_DATA8(instance, 0x29);
    ST7789V2_WR_DATA8(instance, 0x33);
    ST7789V2_WR_DATA8(instance, 0x3E);
    ST7789V2_WR_DATA8(instance, 0x38);
    ST7789V2_WR_DATA8(instance, 0x12);
    ST7789V2_WR_DATA8(instance, 0x12);
    ST7789V2_WR_DATA8(instance, 0x28);
    ST7789V2_WR_DATA8(instance, 0x30);

    ST7789V2_WR_REG(instance, 0xE1);
    ST7789V2_WR_DATA8(instance, 0xF0);
    ST7789V2_WR_DATA8(instance, 0x07);
    ST7789V2_WR_DATA8(instance, 0x0A);
    ST7789V2_WR_DATA8(instance, 0x0D);
    ST7789V2_WR_DATA8(instance, 0x0B);
    ST7789V2_WR_DATA8(instance, 0x07);
    ST7789V2_WR_DATA8(instance, 0x28);
    ST7789V2_WR_DATA8(instance, 0x33);
    ST7789V2_WR_DATA8(instance, 0x3E);
    ST7789V2_WR_DATA8(instance, 0x36);
    ST7789V2_WR_DATA8(instance, 0x14);
    ST7789V2_WR_DATA8(instance, 0x14);
    ST7789V2_WR_DATA8(instance, 0x29);
    ST7789V2_WR_DATA8(instance, 0x32);

    ST7789V2_WR_REG(instance, 0x21);

    ST7789V2_WR_REG(instance, 0x11);
    ST7789V2_DELAY_XMS(120);
    ST7789V2_WR_REG(instance, 0x29);
}

/**
 * @brief   在指定区域填充颜色
 * @param   xsta,ysta   起始坐标
 *          xend,yend   终止坐标
 *          color       要填充的颜色
 * @retval
 **/
void ST7789V2_Fill(ST7789V2_Instance *instance, uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    ST7789V2_Address_Set(instance, xsta, ysta, xend - 1, yend - 1); // 设置显示范围
    for (i = ysta; i < yend; i++)
    {
        for (j = xsta; j < xend; j++)
        {
            ST7789V2_WR_DATA(instance, color);
        }
    }
}

/**
 * @brief   在指定位置画点
 * @param   x,y 画点坐标
 *          color 点的颜色
 * @retval
 **/
void ST7789V2_DrawPoint(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint16_t color)
{
    ST7789V2_Address_Set(instance, x, y, x, y); // 设置光标位置
    ST7789V2_WR_DATA(instance, color);
}

/**
 * @brief   画线
 * @param   x1,y1   起始坐标
 *          x2,y2   终止坐标
 *          color   线的颜色
 * @retval
 **/
void ST7789V2_DrawLine(ST7789V2_Instance *instance, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    uRow = x1; // 画线起点坐标
    uCol = y1;
    if (delta_x > 0)
        incx = 1; // 设置单步方向
    else if (delta_x == 0)
        incx = 0; // 垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // 水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if (delta_x > delta_y)
        distance = delta_x; // 选取基本增量坐标轴
    else
        distance = delta_y;
    for (t = 0; t < distance + 1; t++)
    {
        ST7789V2_DrawPoint(instance, uRow, uCol, color); // 画点
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}

/**
 * @brief   画矩形
 * @param   x1,y1   起始坐标
 *          x2,y2   终止坐标
 *          color   线的颜色
 * @retval
 **/
void ST7789V2_DrawRectangle(ST7789V2_Instance *instance, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    ST7789V2_DrawLine(instance, x1, y1, x2, y1, color);
    ST7789V2_DrawLine(instance, x1, y1, x1, y2, color);
    ST7789V2_DrawLine(instance, x1, y2, x2, y2, color);
    ST7789V2_DrawLine(instance, x2, y1, x2, y2, color);
}

/**
 * @brief   画圆
 * @param   x0,y0   圆心坐标
 *          r       半径
 *          color   圆的颜色
 * @retval
 **/
void ST7789V2_DrawCircle(ST7789V2_Instance *instance, uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
    int a, b;
    a = 0;
    b = r;
    while (a <= b)
    {
        ST7789V2_DrawPoint(instance, x0 - b, y0 - a, color); // 3
        ST7789V2_DrawPoint(instance, x0 + b, y0 - a, color); // 0
        ST7789V2_DrawPoint(instance, x0 - a, y0 + b, color); // 1
        ST7789V2_DrawPoint(instance, x0 - a, y0 - b, color); // 2
        ST7789V2_DrawPoint(instance, x0 + b, y0 + a, color); // 4
        ST7789V2_DrawPoint(instance, x0 + a, y0 - b, color); // 5
        ST7789V2_DrawPoint(instance, x0 + a, y0 + b, color); // 6
        ST7789V2_DrawPoint(instance, x0 - b, y0 + a, color); // 7
        a++;
        if ((a * a + b * b) > (r * r)) // 判断要画的点是否过远
        {
            b--;
        }
    }
}
/**
 * @brief   显示汉字串
 * @param   x,y 显示坐标
 *          *s  要显示的汉字串
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号 可选 16 24 32
 *          mode:  0非叠加模式  1叠加模式
 * @retval
 **/
void ST7789V2_ShowChinese(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    while (*s != 0)
    {
        if (sizey == 12)
            ST7789V2_ShowChinese12x12(instance, x, y, s, fc, bc, sizey, mode);
        else if (sizey == 16)
            ST7789V2_ShowChinese16x16(instance, x, y, s, fc, bc, sizey, mode);
        else if (sizey == 24)
            ST7789V2_ShowChinese24x24(instance, x, y, s, fc, bc, sizey, mode);
        else if (sizey == 32)
            ST7789V2_ShowChinese32x32(instance, x, y, s, fc, bc, sizey, mode);
        else
            return;
        s += 2;
        x += sizey;
    }
}

/**
 * @brief   显示单个12x12汉字
 * @param   x,y 显示坐标
 *          *s  要显示的汉字串
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号 可选 16 24 32
 *          mode:  0非叠加模式  1叠加模式
 * @retval
 **/
void ST7789V2_ShowChinese12x12(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;       // 汉字数目
    uint16_t TypefaceNum; // 一个字符所占字节大小
    uint16_t x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;

    HZnum = sizeof(tfont12) / sizeof(typFNT_GB12); // 统计汉字数目
    for (k = 0; k < HZnum; k++)
    {
        if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1)))
        {
            ST7789V2_Address_Set(instance, x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j))
                            ST7789V2_WR_DATA(instance, fc);
                        else
                            ST7789V2_WR_DATA(instance, bc);
                        m++;
                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else // 叠加方式
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j))
                            ST7789V2_DrawPoint(instance, x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/**
 * @brief   显示单个16x16汉字
 * @param   x,y 显示坐标
 *          *s  要显示的汉字串
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号 可选 16 24 32
 *          mode:  0非叠加模式  1叠加模式
 * @retval
 **/
void ST7789V2_ShowChinese16x16(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;       // 汉字数目
    uint16_t TypefaceNum; // 一个字符所占字节大小
    uint16_t x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont16) / sizeof(typFNT_GB16); // 统计汉字数目
    for (k = 0; k < HZnum; k++)
    {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)))
        {
            ST7789V2_Address_Set(instance, x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))
                            ST7789V2_WR_DATA(instance, fc);
                        else
                            ST7789V2_WR_DATA(instance, bc);
                        m++;
                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else // 叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))
                            ST7789V2_DrawPoint(instance, x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/**
 * @brief   显示单个24x24汉字
 * @param   x,y 显示坐标
 *          *s  要显示的汉字串
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号 可选 16 24 32
 *          mode:  0非叠加模式  1叠加模式
 * @retval
 **/
void ST7789V2_ShowChinese24x24(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;       // 汉字数目
    uint16_t TypefaceNum; // 一个字符所占字节大小
    uint16_t x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24); // 统计汉字数目
    for (k = 0; k < HZnum; k++)
    {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
        {
            ST7789V2_Address_Set(instance, x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                            ST7789V2_WR_DATA(instance, fc);
                        else
                            ST7789V2_WR_DATA(instance, bc);
                        m++;
                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else // 叠加方式
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                            ST7789V2_DrawPoint(instance, x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/**
 * @brief   显示单个32x32汉字
 * @param   x,y 显示坐标
 *          *s  要显示的汉字串
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号 可选 16 24 32
 *          mode:  0非叠加模式  1叠加模式
 * @retval
 **/
void ST7789V2_ShowChinese32x32(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t *s, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t i, j, m = 0;
    uint16_t k;
    uint16_t HZnum;       // 汉字数目
    uint16_t TypefaceNum; // 一个字符所占字节大小
    uint16_t x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32); // 统计汉字数目
    for (k = 0; k < HZnum; k++)
    {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
        {
            ST7789V2_Address_Set(instance, x, y, x + sizey - 1, y + sizey - 1);
            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) // 非叠加方式
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                            ST7789V2_WR_DATA(instance, fc);
                        else
                            ST7789V2_WR_DATA(instance, bc);
                        m++;
                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else // 叠加方式
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                            ST7789V2_DrawPoint(instance, x, y, fc); // 画一个点
                        x++;
                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/**
 * @brief   显示单个字符
 * @param   x,y 显示坐标
 *          num 要显示的字符
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号 可选 16 24 32
 *          mode:  0非叠加模式  1叠加模式
 * @retval
 **/
void ST7789V2_ShowChar(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    uint8_t temp, sizex, t, m = 0;
    uint16_t i, TypefaceNum; // 一个字符所占字节大小
    uint16_t x0 = x;
    sizex = sizey / 2;
    TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    num = num - ' ';                                                    // 得到偏移后的值
    ST7789V2_Address_Set(instance, x, y, x + sizex - 1, y + sizey - 1); // 设置光标位置
    for (i = 0; i < TypefaceNum; i++)
    {
        if (sizey == 12)
            temp = ascii_1206[num][i]; // 调用6x12字体
        else if (sizey == 16)
            temp = ascii_1608[num][i]; // 调用8x16字体
        else if (sizey == 24)
            temp = ascii_2412[num][i]; // 调用12x24字体
        else if (sizey == 32)
            temp = ascii_3216[num][i]; // 调用16x32字体
        else
            return;
        for (t = 0; t < 8; t++)
        {
            if (!mode) // 非叠加模式
            {
                if (temp & (0x01 << t))
                    ST7789V2_WR_DATA(instance, fc);
                else
                    ST7789V2_WR_DATA(instance, bc);
                m++;
                if (m % sizex == 0)
                {
                    m = 0;
                    break;
                }
            }
            else // 叠加模式
            {
                if (temp & (0x01 << t))
                    ST7789V2_DrawPoint(instance, x, y, fc); // 画一个点
                x++;
                if ((x - x0) == sizex)
                {
                    x = x0;
                    y++;
                    break;
                }
            }
        }
    }
}

/**
 * @brief   显示字符串
 * @param   x,y 显示坐标
 *          *p 要显示的字符串
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号 可选 16 24 32
 *          mode:  0非叠加模式  1叠加模式
 * @retval
 **/
void ST7789V2_ShowString(ST7789V2_Instance *instance, uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc, uint16_t bc, uint8_t sizey, uint8_t mode)
{
    while (*p != '\0')
    {
        ST7789V2_ShowChar(instance, x, y, *p, fc, bc, sizey, mode);
        x += sizey / 2;
        p++;
    }
}

/**
 * @brief   数字
 * @param   m底数，n指数
 * @retval
 **/
uint32_t mypow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)
        result *= m;
    return result;
}

/**
 * @brief   显示整数变量
 * @param   x,y 显示坐标
 *          num 要显示整数变量
 *          len 要显示的位数
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号
 * @retval
 **/
void ST7789V2_ShowIntNum(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;
    for (t = 0; t < len; t++)
    {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                ST7789V2_ShowChar(instance, x + t * sizex, y, ' ', fc, bc, sizey, 0);
                continue;
            }
            else
                enshow = 1;
        }
        ST7789V2_ShowChar(instance, x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

/**
 * @brief   显示两位小数变量
 * @param   x,y 显示坐标
 *          num 要显示小数变量
 *          len 要显示的位数
 *          fc 字的颜色
 *          bc 字的背景色
 *          sizey 字号
 * @retval
 **/
void ST7789V2_ShowFloatNum1(ST7789V2_Instance *instance, uint16_t x, uint16_t y, float num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    uint8_t t, temp, sizex;
    uint16_t num1;
    sizex = sizey / 2;
    num1 = num * 100;
    for (t = 0; t < len; t++)
    {
        temp = (num1 / mypow(10, len - t - 1)) % 10;
        if (t == (len - 2))
        {
            ST7789V2_ShowChar(instance, x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
            t++;
            len += 1;
        }
        ST7789V2_ShowChar(instance, x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}

/**
 * @brief   显示图片
 * @param   x,y起点坐标
 *          length 图片长度
 *          width  图片宽度
 *          pic[]  图片数组
 * @retval
 **/
void ST7789V2_ShowPicture(ST7789V2_Instance *instance, uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t pic[])
{
    uint16_t i, j;
    uint32_t k = 0;
    ST7789V2_Address_Set(instance, x, y, x + length - 1, y + width - 1);
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < width; j++)
        {
            ST7789V2_WR_DATA8(instance, pic[k * 2]);
            ST7789V2_WR_DATA8(instance, pic[k * 2 + 1]);
            k++;
        }
    }
}
