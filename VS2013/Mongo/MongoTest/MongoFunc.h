#ifndef MongoFunc_h__
#define MongoFunc_h__

#include <string>
#include <vector>
#include <bson.h>

int Sample(void);

void TestMongo(void);

void Insert(void);
void Delete(void);
void Load(void);
void Update(void);
void Count(void);
void CreateIndex(void);
void FindAndModify(void);
void FindAndModifyDel(void);
void FindAndModifyUpdate(void);
void BsonTest(void);

inline bool BsonIterValue(const bson_t* doc, const std::string key, bson_iter_t& ival);
inline bool BsonJson(const bson_t* doc, const std::string key, std::string& val);
inline bool BsonValue(const bson_t* doc, const std::string key, bool& val);
inline bool BsonValue(const bson_t* doc, const std::string key, int& val);
inline bool BsonValue(const bson_t* doc, const std::string key, double& val);
inline bool BsonValue(const bson_t* doc, const std::string key, std::string& val);
inline bool BsonValue(const bson_t* doc, const std::string key, std::vector<std::string>& val);



#endif // MongoFunc_h__
