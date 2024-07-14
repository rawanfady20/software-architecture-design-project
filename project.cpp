
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Abstract base class for the prototype
class Student {
public:
    virtual std::shared_ptr<Student> clone() const = 0;
    virtual bool canTakeCourse(const std::string& course) const = 0;
    virtual bool hasTestToSkipLevels() const = 0;
    virtual const std::vector<std::string>& getCategories() const = 0;
    virtual ~Student() {}
};

// Concrete prototype class
class BasicStudent : public Student {
private:
    std::vector<std::string> categories;
    bool testToSkipLevels;

public:
    BasicStudent(const std::vector<std::string>& categories, bool testToSkipLevels)
        : categories(categories), testToSkipLevels(testToSkipLevels) {}

    std::shared_ptr<Student> clone() const override {
        return std::make_shared<BasicStudent>(*this);
    }

    bool canTakeCourse(const std::string& course) const override {
        // Simplified logic for demonstration
        return true;
    }

    bool hasTestToSkipLevels() const override {
        return testToSkipLevels;
    }

    const std::vector<std::string>& getCategories() const override {
        return categories;
    }
};

// Student Decorator base class
class StudentDecorator : public Student {
protected:
    std::shared_ptr<Student> baseStudent;

public:
    StudentDecorator(std::shared_ptr<Student> baseStudent) : baseStudent(baseStudent) {}

    std::shared_ptr<Student> clone() const override {
        return baseStudent->clone(); // Delegate cloning to the wrapped object
    }

    bool canTakeCourse(const std::string& course) const override {
        return baseStudent->canTakeCourse(course); // Delegate course checking to the wrapped object
    }

    bool hasTestToSkipLevels() const override {
        return baseStudent->hasTestToSkipLevels(); // Delegate test checking to the wrapped object
    }

    const std::vector<std::string>& getCategories() const override {
        return baseStudent->getCategories(); // Delegate category retrieval to the wrapped object
    }
};

// Specific decorator to enhance functionality
class TutoringSupportDecorator : public StudentDecorator {
public:
    TutoringSupportDecorator(std::shared_ptr<Student> baseStudent) : StudentDecorator(baseStudent) {}

    bool canTakeCourse(const std::string& course) const override {
        // Assume tutoring support enhances course-taking ability
        return true; // Enhanced functionality
    }
};

// Factory Pattern Interface
class StudentFactory {
public:
    virtual std::shared_ptr<Student> createStudent(const std::vector<std::string>& categories, bool testToSkipLevels) = 0;
};

// Concrete Factory
class BasicStudentFactory : public StudentFactory {
public:
    std::shared_ptr<Student> createStudent(const std::vector<std::string>& categories, bool testToSkipLevels) override {
        return std::make_shared<BasicStudent>(categories, testToSkipLevels);
    }
};

// Builder Pattern for BasicStudent
class BasicStudentBuilder {
private:
    std::vector<std::string> categories;
    bool testToSkipLevels = false;

public:
    BasicStudentBuilder& setCategories(const std::vector<std::string>& categories) {
        this->categories = categories;
        return *this;
    }

    BasicStudentBuilder& setTestToSkipLevels(bool testToSkipLevels) {
        this->testToSkipLevels = testToSkipLevels;
        return *this;
    }

    std::shared_ptr<Student> build() {
        return std::make_shared<BasicStudent>(categories, testToSkipLevels);
    }
};

// Singleton Pattern for managing university students
class University {
private:
    std::vector<std::shared_ptr<Student>> students;

    University() {} // Private constructor to prevent instantiation

public:
    static University& getInstance() {
        static University instance; // Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }

    void addStudent(std::shared_ptr<Student> student) {
        students.push_back(student);
    }

    std::vector<std::shared_ptr<Student>> getStudents() const {
        return students;
    }
};

int main() {
    BasicStudentFactory factory;
    std::cout << "Factory Pattern: Created a BasicStudent using BasicStudentFactory.\n";
    auto student1 = factory.createStudent({ "Math", "Physics" }, true);

    std::cout << "Decorator Pattern: Enhancing BasicStudent with TutoringSupportDecorator.\n";
    auto tutoredStudent1 = std::make_shared<TutoringSupportDecorator>(student1);

    std::cout << "Singleton Pattern: Adding student to the University (Singleton).\n";
    University::getInstance().addStudent(tutoredStudent1);

    std::cout << "University now has " << University::getInstance().getStudents().size() << " students.\n";

    std::cout << "Checking enhanced capabilities due to Decorator: Can tutored student take 'Advanced Quantum Mechanics'? "
        << std::boolalpha << tutoredStudent1->canTakeCourse("Advanced Quantum Mechanics") << std::endl;

    return 0;
} 