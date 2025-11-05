#include "gtest/gtest.h"
#include "services/client_repository.h"
#include "entities/client.h"

using namespace computer_club::services;
using namespace computer_club::entities;

class ClientRepositoryTest : public ::testing::Test {
protected:
    ClientRepository repo_;
};

TEST_F(ClientRepositoryTest, InitialState) {
    EXPECT_FALSE(repo_.HasClient("client1"));
    EXPECT_EQ(repo_.GetClient("client1"), nullptr);
    EXPECT_TRUE(repo_.GetClientNames().empty());
}

TEST_F(ClientRepositoryTest, AddClient) {
    std::string client_name = "client1";
    repo_.AddClient(client_name);

    EXPECT_TRUE(repo_.HasClient(client_name));
    auto client = repo_.GetClient(client_name);
    ASSERT_NE(client, nullptr);
    EXPECT_EQ(client->GetName(), client_name);

    auto names = repo_.GetClientNames();
    ASSERT_EQ(names.size(), 1);
    EXPECT_EQ(names[0], client_name);
}

TEST_F(ClientRepositoryTest, AddDuplicateClient) {
    std::string client_name = "client1";
    repo_.AddClient(client_name);
    auto client_ptr_1 = repo_.GetClient(client_name);

    repo_.AddClient(client_name);
    auto client_ptr_2 = repo_.GetClient(client_name);

    EXPECT_TRUE(repo_.HasClient(client_name));
    EXPECT_EQ(repo_.GetClientNames().size(), 1);
    EXPECT_EQ(client_ptr_1, client_ptr_2);
}

TEST_F(ClientRepositoryTest, AddMultipleClients) {
    repo_.AddClient("Y");
    repo_.AddClient("B");

    EXPECT_TRUE(repo_.HasClient("Y"));
    EXPECT_TRUE(repo_.HasClient("B"));
    EXPECT_FALSE(repo_.HasClient("client3"));

    auto names = repo_.GetClientNames();
    ASSERT_EQ(names.size(), 2);
    EXPECT_EQ(names[0], "B");
    EXPECT_EQ(names[1], "Y");
}

TEST_F(ClientRepositoryTest, RemoveClient) {
    std::string client_name = "client1";
    repo_.AddClient(client_name);
    EXPECT_TRUE(repo_.HasClient(client_name));

    repo_.RemoveClient(client_name);
    EXPECT_FALSE(repo_.HasClient(client_name));
    EXPECT_EQ(repo_.GetClient(client_name), nullptr);
    EXPECT_TRUE(repo_.GetClientNames().empty());
}

TEST_F(ClientRepositoryTest, RemoveNonExistentClient) {
    repo_.AddClient("client1");

    repo_.RemoveClient("client2");

    EXPECT_TRUE(repo_.HasClient("client1"));
    EXPECT_EQ(repo_.GetClientNames().size(), 1);
}