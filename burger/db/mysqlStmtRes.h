#ifndef MYSQLSTMTRES_H
#define MYSQLSTMTRES_H

#include <memory>
#include <vector>
#include "DB.h"

namespace burger {
namespace db {

class MySQLStmtRes : public ISQLData {
friend class MySQLStmt;
public:
    using ptr = std::shared_ptr<MySQLStmtRes>;
    static MySQLStmtRes::ptr Create(std::shared_ptr<MySQLStmt> stmt);
    ~MySQLStmtRes();

    int getErrno() const { return errno_;}
    const std::string& getErrStr() const { return errstr_;}

    int getDataCount() override;
    int getColumnCount() override;
    int getColumnBytes(int idx) override;
    int getColumnType(int idx) override;
    std::string getColumnName(int idx) override;

    bool isNull(int idx) override;
    int8_t getInt8(int idx) override;
    uint8_t getUint8(int idx) override;
    int16_t getInt16(int idx) override;
    uint16_t getUint16(int idx) override;
    int32_t getInt32(int idx) override;
    uint32_t getUint32(int idx) override;
    int64_t getInt64(int idx) override;
    uint64_t getUint64(int idx) override;
    float getFloat(int idx) override;
    double getDouble(int idx) override;
    std::string getString(int idx) override;
    std::string getBlob(int idx) override;
    time_t getTime(int idx) override;
    bool next() override;
private:
    MySQLStmtRes(std::shared_ptr<MySQLStmt> stmt, int eno, const std::string& estr);
    struct Data {
        Data();
        ~Data();

        void alloc(size_t size);

        my_bool is_null;
        my_bool error;
        enum_field_types type;
        unsigned long length;
        int32_t data_length;
        char* data;
    };
private:
    int errno_;
    std::string errstr_;
    std::shared_ptr<MySQLStmt> stmt_;
    std::vector<MYSQL_BIND> binds_;
    std::vector<Data> datas_;
};

} // namespace db

} // namespace burger



#endif // MYSQLSTMTRES_H