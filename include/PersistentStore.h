#ifndef SLATE_PERSISTENT_STORE_H
#define SLATE_PERSISTENT_STORE_H

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/dynamodb/DynamoDBClient.h>


#include "Entities.h"

class PersistentStore{
public:
	PersistentStore(Aws::Auth::AWSCredentials credentials, 
					Aws::Client::ClientConfiguration clientConfig);
	
	///Store a record for a new user
	///\return Whether the user record was successfully added to the database
	bool addUser(const User& user);
	
	///Find information about the user with a given ID
	///\param id the users ID
	///\return the corresponding user or an invalid user object if the id is not known
	User getUser(const std::string& id);
	
	///Find the user who owns the given access token. Currently does not bother 
	///to retreive the user's name, email address, or globus ID. 
	///\param token access token
	///\return the token owner or an invalid user object if the token is not known
	User findUserByToken(const std::string& token);
	
	///Find the user corresponding to the given Globus ID. Currently does not bother 
	///to retreive the user's name, email address, or admin status. 
	///\param globusID Globus ID to look up
	///\return the corresponding user or an invalid user object if the ID is not known
	User findUserByGlobusID(const std::string& globusID);
	
	///Change a user record
	///\return Whether the user record was successfully altered in the database
	bool updateUser(const User& user);
	
	///Delete a user record
	///\return Whether the user record was successfully removed from the database
	bool removeUser(const std::string& id);
	
	///Compile a list of all current user records
	///\return all users, but with only IDs, names, and email addresses
	std::vector<User> listUsers();
	
	bool addUserToVO(const std::string& uID, const std::string voID);
	
	bool removeUserFromVO(const std::string& uID, const std::string& voID);
	
	std::vector<std::string> getUserVOMemberships(const std::string& uID);
	
	bool userInVO(const std::string& uID, const std::string& voID);
	
	//----
	
	bool addVO(const VO& vo);
	
	///Delete a VO record
	///\return Whether the user record was successfully removed from the database
	bool removeVO(const std::string& voID);
	
	std::vector<std::string> getMembersOfVO(const std::string voID);
	
	std::vector<VO> listVOs();
	
	///Find the VO, if any, with the given ID
	///\param name the ID to look up
	///\return the VO corresponding to the ID, or an invalid VO if none exists
	VO findVOByID(const std::string& id);
	
	///Find the VO, if any, with the given name
	///\param name the name to look up
	///\return the VO corresponding to the name, or an invalid VO if none exists
	VO findVOByName(const std::string& name);
	
	///Find the VO, if any, with the given UUID or name
	///\param idOrName the UUID or name of the VO to look up
	///\return the VO corresponding to the name, or an invalid VO if none exists
	VO getVO(const std::string& idOrName);
	
	//----
	
	///Store a record for a new cluster
	///\return Whether the record was successfully added to the database
	bool addCluster(const Cluster& cluster);
	
	///Delete a cluster record
	///\return Whether the user record was successfully removed from the database
	bool removeCluster(const std::string& cID);
	
	std::vector<Cluster> listClusters();
	
	///For consumption by kubectl, we store them in the filesystem rather than 
	///the database.
	///\return the path for the given cluser's config file
	std::string configPathForCluster(const std::string& cID);
	
	///Find the cluster, if any, with the given ID
	///\param name the ID to look up
	///\return the cluster corresponding to the ID, or an invalid cluster if 
	///        none exists
	Cluster findClusterByID(const std::string& id);
	
	///Find the cluster, if any, with the given name
	///\param name the name to look up
	///\return the cluster corresponding to the name, or an invalid cluster if 
	///        none exists
	Cluster findClusterByName(const std::string& name);
	
	///Find the cluster, if any, with the given UUID or name
	///\param idOrName the UUID or name of the cluster to look up
	///\return the cluster corresponding to the name, or an invalid cluster if 
	///        none exists
	Cluster getCluster(const std::string& idOrName);
	
	//----
	
	///Store a record for a new application instance
	///\return Whether the record was successfully added to the database
	bool addApplicationInstance(const ApplicationInstance& inst);
	
	///Delete a user record
	///\return Whether the user record was successfully removed from the database
	bool removeApplicationInstance(const std::string& id);
	
	///Find information about the application instance with a given ID
	///\param id the instance ID
	///\return the corresponding instance or an invalid application instance 
	///        object if the id is not known. If found, the instance's config
	///        will not be set; it must be fetched using 
	///        getApplicationInstanceConfig
	ApplicationInstance getApplicationInstance(const std::string& id);
	
	///Get the configuration information for an application instance with a 
	///given ID
	///\param id the instance ID
	///\return the corresponding instance configuration, or the empty string if
	///        the id is not known
	std::string getApplicationInstanceConfig(const std::string& id);
	
private:
	Aws::DynamoDB::DynamoDBClient dbClient;
	const std::string userTableName;
	const std::string voTableName;
	const std::string clusterTableName;
	const std::string instanceTableName;
	
	void InitializeTables();
};

#endif //SLATE_PERSISTENT_STORE_H