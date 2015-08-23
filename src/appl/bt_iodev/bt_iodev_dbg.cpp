/******************************************************************************
 *   Description:   TODO
 *
 ******************************************************************************/

/******************************************************************************
 * INCLUDED FILES
 ******************************************************************************/

#include <QDebug>

#include "bt_iodev_dbg.h"
#include "my_util.h"

#include <vector>
#include <stdexcept>
#include <tuple>


using namespace std;

/*******************************************************************************
 * CONSTRUCTOR, DESTRUCTOR
 ******************************************************************************/

IODevDbg::IODevDbg() :
    timer(this),
    cur_data(),
    stage(0),
    opened(false)
{
    connect(&timer, &QTimer::timeout, this, &IODevDbg::nextMsgGenerate);
}

IODevDbg::~IODevDbg()
{
}



/*******************************************************************************
 * STATIC METHODS
 ******************************************************************************/

/* private      */

/* protected    */

/* public       */


/*******************************************************************************
 * METHODS
 ******************************************************************************/

/* private      */

/* protected    */

/* public       */

vector<uint8_t> IODevDbg::read()
{
    vector<uint8_t> ret = std::move(cur_data);
    cur_data = vector<uint8_t>{};
    return ret;
}

void IODevDbg::write(const vector<uint8_t> &data)
{
    //std::ignore = data;
    qDebug() << "write: " << MyUtil::byteArrayToHex(data);
}

void IODevDbg::setBaudRate(int32_t baud_rate)
{
    std::ignore = baud_rate;
    //-- do nothing
}

void IODevDbg::open()
{
    opened = true;

    timer.start(200);

    emit error("Just test error. Don't worry.");
    emit openStatusChanged(true);
}

void IODevDbg::close()
{
    opened = false;
    timer.stop();
    emit openStatusChanged(false);
}

bool IODevDbg::isOpened()
{
    return opened;
}

QString IODevDbg::toString()
{
    return "Debug IO device";
}



/*******************************************************************************
 * SLOTS
 ******************************************************************************/

/* private      */

void IODevDbg::nextMsgGenerate()
{
    vector<uint8_t> new_data;

    switch (stage){
        case 0:
            new_data = {0x31, 0x80};
            break;
        case 1:
            new_data = {0x03, 0x02, 0x01};
            break;
        case 2:
            new_data = {0x04, 0x05, 0x06, 0x95,   0x07};
            break;
    }

    stage++;
    if (stage > 2){
        stage = 0;
    }

    cur_data.insert(
            cur_data.end(),
            new_data.cbegin(), new_data.cend()
            );

    emit readyRead(cur_data.size());
}

/* protected    */

/* public       */


