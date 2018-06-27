#ifndef SLATE_ENTITIES_H
#define SLATE_ENTITIES_H

#include <mutex>
#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>

///Represents a user account
struct User{
	User():valid(false),admin(false){}
	explicit User(std::string name):
	valid(true),name(std::move(name)){}
	
	///Indicates whether the account exists/is valid
	bool valid;
	std::string id;
	std::string name;
	std::string email;
	std::string token;
	std::string globusID;
	bool admin;
	
	explicit operator bool() const{ return valid; }
};

bool operator==(const User& u1, const User& u2);
bool operator!=(const User& u1, const User& u2);
std::ostream& operator<<(std::ostream& os, const User& u);

struct VO{
	VO():valid(false){}
	explicit VO(std::string name):valid(true),name(std::move(name)){}
	
	///Indicates whether the VO exists/is valid
	bool valid;
	std::string id;
	std::string name;
	
	explicit operator bool() const{ return valid; }
};

std::ostream& operator<<(std::ostream& os, const VO& vo);

struct Cluster{
	Cluster():valid(false){}
	explicit Cluster(std::string name):valid(true),name(std::move(name)){}
	
	///Indicates whether the cluster exists/is valid
	bool valid;
	std::string id;
	std::string name;
	std::string config;
	std::string owningVO;
	
	explicit operator bool() const{ return valid; }
};

std::ostream& operator<<(std::ostream& os, const Cluster& c);

///Represents a deployable application
struct Application{
	Application():valid(false){}
	explicit Application(std::string name):valid(true),name(std::move(name)){}
	
	///Indicates whether the application exists/is valid
	bool valid;
	std::string name;
	
	explicit operator bool() const{ return valid; }
	
	enum Repository{
		MainRepository,
		DevelopmentRepository
	};
};

std::ostream& operator<<(std::ostream& os, const Application& a);

///Represents a deployed/running application instance
struct ApplicationInstance{
	ApplicationInstance():valid(false){}
	
	///Indicates whether the instance exists/is valid
	bool valid;
	std::string id;
	std::string name;
	std::string owningVO;
	std::string cluster;
	std::string config;
	std::string ctime;
	
	explicit operator bool() const{ return valid; }
};

std::ostream& operator<<(std::ostream& os, const ApplicationInstance& a);

static class IDGenerator{
public:
	///Creates a random ID for a new user
	std::string generateUserID(){
		std::lock_guard<std::mutex> lock(mut);
		boost::uuids::uuid id = gen();
		return userIDPrefix+to_string(id);
	}
	///Creates a random ID for a new cluster
	std::string generateClusterID(){
		std::lock_guard<std::mutex> lock(mut);
		boost::uuids::uuid id = gen();
		return clusterIDPrefix+to_string(id);
	}
	///Creates a random ID for a new VO
	std::string generateVOID(){
		std::lock_guard<std::mutex> lock(mut);
		boost::uuids::uuid id = gen();
		return voIDPrefix+to_string(id);
	}
	///Creates a random ID for a new application instance
	std::string generateInstanceID(){
		std::lock_guard<std::mutex> lock(mut);
		boost::uuids::uuid id = gen();
		return instanceIDPrefix+to_string(id);
	}
	///Creates a random access token for a user
	///At the moment there is no apparent reason that a user's access token
	///should have any particular structure or meaning. Definite requirements:
	/// - Each user's token should be unique
	/// - There should be no way for anyone to derive or guess a user's token
	///These requirements seem adequately satisfied by a block of 
	///cryptographically random data. Note that boost::uuids::random_generator 
	///uses /dev/urandom as a source of randomness, so this is not optimally
	///secure on Linux hosts. 
	std::string generateUserToken(){
		std::lock_guard<std::mutex> lock(mut);
		boost::uuids::uuid id = gen();
		return to_string(id);
	}
	
	const static std::string userIDPrefix;
	const static std::string clusterIDPrefix;
	const static std::string voIDPrefix;
	const static std::string instanceIDPrefix;
	
private:
	std::mutex mut;
	boost::uuids::random_generator gen;
} idGenerator;

#endif //SLATE_ENTITIES_H