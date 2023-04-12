#pragma once

namespace features {

struct IFeature {

    virtual void create_variables() = 0;
    virtual void save_variables() = 0;
    virtual void load_variables() = 0;
    virtual void setup_hooks() = 0;
    virtual void uninitialize() = 0;

};

}

