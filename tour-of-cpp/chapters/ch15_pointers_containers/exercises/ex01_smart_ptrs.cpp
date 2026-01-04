// Exercise 15.1: Smart Pointer Practice
// Book reference: 15.2 Pointers
// Difficulty: **
//
// Instructions:
// Implement the TODOs below to practice smart pointer usage.
//
// Part 1: Implement a simple resource manager using unique_ptr
// Part 2: Implement a shared document system using shared_ptr and weak_ptr
// Part 3: Implement a factory function with custom deleter
//
// Compile and run to verify your solutions.

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cassert>

// ============================================================================
// Part 1: Resource Manager with unique_ptr
// ============================================================================

class DatabaseConnection {
public:
    DatabaseConnection(std::string host) : host_{std::move(host)}, connected_{false} {
        std::cout << "DatabaseConnection created for " << host_ << "\n";
    }

    ~DatabaseConnection() {
        if (connected_) {
            disconnect();
        }
        std::cout << "DatabaseConnection destroyed for " << host_ << "\n";
    }

    void connect() {
        connected_ = true;
        std::cout << "Connected to " << host_ << "\n";
    }

    void disconnect() {
        connected_ = false;
        std::cout << "Disconnected from " << host_ << "\n";
    }

    bool is_connected() const { return connected_; }
    const std::string& host() const { return host_; }

private:
    std::string host_;
    bool connected_;
};

// TODO 1: Implement create_connection
// Create and return a unique_ptr to a DatabaseConnection
// The connection should be connected before returning
std::unique_ptr<DatabaseConnection> create_connection(const std::string& host) {
    // Your implementation here
    return nullptr;  // Replace this
}

// TODO 2: Implement transfer_connection
// Take ownership of a connection and return it with the host modified
// (Simulate reconnecting to a different server)
std::unique_ptr<DatabaseConnection> transfer_connection(
    std::unique_ptr<DatabaseConnection> conn,
    const std::string& new_host) {
    // Your implementation here
    // Hint: Create a new connection to new_host, disconnect the old one
    return nullptr;  // Replace this
}

void test_part1() {
    std::cout << "=== Part 1: unique_ptr Resource Manager ===\n";

    auto conn = create_connection("localhost");
    assert(conn != nullptr);
    assert(conn->is_connected());
    assert(conn->host() == "localhost");

    auto new_conn = transfer_connection(std::move(conn), "remotehost");
    assert(conn == nullptr);  // Old pointer should be null
    assert(new_conn != nullptr);
    assert(new_conn->host() == "remotehost");

    std::cout << "Part 1 passed!\n\n";
}

// ============================================================================
// Part 2: Shared Document System
// ============================================================================

class Document;

class Editor {
public:
    Editor(std::string name) : name_{std::move(name)} {}

    // TODO 3: Implement open_document
    // Store a weak_ptr to the document (editors don't own documents)
    void open_document(std::shared_ptr<Document> doc) {
        // Your implementation here
    }

    // TODO 4: Implement get_document
    // Return the shared_ptr if document still exists, nullptr otherwise
    std::shared_ptr<Document> get_document() const {
        // Your implementation here
        return nullptr;  // Replace this
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
    // TODO: Add a weak_ptr member to store the document reference
};

class Document : public std::enable_shared_from_this<Document> {
public:
    Document(std::string title) : title_{std::move(title)} {
        std::cout << "Document '" << title_ << "' created\n";
    }

    ~Document() {
        std::cout << "Document '" << title_ << "' destroyed\n";
    }

    // TODO 5: Implement add_editor
    // Add an editor and give it a reference to this document
    void add_editor(std::shared_ptr<Editor> editor) {
        // Your implementation here
        // Hint: Use shared_from_this()
    }

    const std::string& title() const { return title_; }
    size_t editor_count() const { return editors_.size(); }

private:
    std::string title_;
    std::vector<std::shared_ptr<Editor>> editors_;
};

void test_part2() {
    std::cout << "=== Part 2: Shared Document System ===\n";

    auto editor1 = std::make_shared<Editor>("Alice");
    auto editor2 = std::make_shared<Editor>("Bob");

    {
        auto doc = std::make_shared<Document>("Report.txt");

        doc->add_editor(editor1);
        doc->add_editor(editor2);

        assert(doc->editor_count() == 2);
        assert(editor1->get_document() != nullptr);
        assert(editor1->get_document()->title() == "Report.txt");

        std::cout << "Document has " << doc->editor_count() << " editors\n";
        std::cout << "Closing document...\n";
    }

    // Document should be destroyed now
    assert(editor1->get_document() == nullptr);
    assert(editor2->get_document() == nullptr);
    std::cout << "Editors correctly detect document is gone\n";

    std::cout << "Part 2 passed!\n\n";
}

// ============================================================================
// Part 3: Custom Deleter Factory
// ============================================================================

struct NetworkSocket {
    int fd;
    std::string address;

    NetworkSocket(int f, std::string addr) : fd{f}, address{std::move(addr)} {
        std::cout << "Socket " << fd << " opened to " << address << "\n";
    }
};

// Global counter for testing
inline int sockets_closed = 0;

// Custom deleter that logs and counts socket closures
void close_socket(NetworkSocket* socket) {
    if (socket) {
        std::cout << "Closing socket " << socket->fd << " to " << socket->address << "\n";
        ++sockets_closed;
        delete socket;
    }
}

// TODO 6: Implement create_socket
// Create a unique_ptr<NetworkSocket> with close_socket as the custom deleter
// Use the provided fd and address parameters
auto create_socket(int fd, const std::string& address) {
    // Your implementation here
    // Hint: unique_ptr<NetworkSocket, decltype(&close_socket)>
    return std::unique_ptr<NetworkSocket, decltype(&close_socket)>(nullptr, &close_socket);
}

void test_part3() {
    std::cout << "=== Part 3: Custom Deleter ===\n";

    sockets_closed = 0;

    {
        auto socket1 = create_socket(42, "192.168.1.1:8080");
        auto socket2 = create_socket(43, "192.168.1.2:8080");

        assert(socket1 != nullptr);
        assert(socket2 != nullptr);
        assert(socket1->fd == 42);
        assert(socket2->address == "192.168.1.2:8080");

        std::cout << "Using sockets...\n";
    }

    assert(sockets_closed == 2);
    std::cout << "Both sockets closed via custom deleter\n";
    std::cout << "Part 3 passed!\n\n";
}

int main() {
    test_part1();
    test_part2();
    test_part3();

    std::cout << "=== All exercises completed! ===\n";
    return 0;
}
