#ifndef I_SPECTRE_CONTAINER_H
#define I_SPECTRE_CONTAINER_H

class ISpectreContainer {
public:
    virtual ~ISpectreContainer() {};

    /**
     * @brief set the cursor position
     * @param position The new postition of the cursor
     */
    virtual void SetCursorPos(const int &position) = 0;

    /**
     * @return The current cursor position
     */
    virtual const int &GetCursorPos() = 0;
};

#endif // I_SPECTRE_CONTAINER_H
