/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== init.py ==================
static const unsigned char temp_binary_data_0[] =
"\n"
"inputs = Input([0,1])\n"
"outputs = inputs\n"
"outputs.out()\n"
"\n";

const char* init_py = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x74213398:  numBytes = 38; return init_py;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "init_py"
};

const char* originalFilenames[] =
{
    "init.py"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
    {
        if (namedResourceList[i] == resourceNameUTF8)
            return originalFilenames[i];
    }

    return nullptr;
}

}
