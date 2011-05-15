#ifndef MODEL_H
#define MODEL_H


class Model
{
    public:
        Model();
        Model(const char* inFile);
        virtual ~Model();

        void display();

    protected:

    private:

        void loadC3M(const char* inFile);
};

inline void Model::display()
{

}

#endif // MODEL_H
