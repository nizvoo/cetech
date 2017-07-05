#ifndef CETECH_OS_H
#define CETECH_OS_H


#ifdef __cplusplus
extern "C" {
#endif


//==============================================================================
// Includes
//==============================================================================

#include <stdint.h>
#include <stddef.h>

#include <cetech/celib/math_types.h>

#include "_machine_enums.h"

struct allocator;

//==============================================================================
// Typedefs
//==============================================================================

typedef int (*thread_fce_t)(void *data);

//==============================================================================
// Enums
//==============================================================================

enum vio_open_mode {
    VIO_OPEN_READ,
    VIO_OPEN_WRITE,
};

enum vio_seek {
    VIO_SEEK_SET,
    VIO_SEEK_CUR,
    VIO_SEEK_END
};

enum window_flags {
    WINDOW_NOFLAG = 0,
    WINDOW_FULLSCREEN = 1,
};

enum window_pos {
    WINDOWPOS_CENTERED = 1,
    WINDOWPOS_UNDEFINED = 2
};

//==============================================================================
// Structs
//==============================================================================

typedef void os_thread_t;

typedef struct {
    int lock;
} os_spinlock_t;

typedef void os_window_t;

typedef void os_vio_instance_v0;

struct os_vio  {
    os_vio_instance_v0* inst;

    int64_t (*size)(os_vio_instance_v0 *vio);

    int64_t (*seek)(os_vio_instance_v0 *vio,
                    int64_t offset,
                    enum vio_seek whence);

    size_t (*read)(os_vio_instance_v0 *vio,
                   void *ptr,
                   size_t size,
                   size_t maxnum);

    size_t (*write)(os_vio_instance_v0 *vio,
                    const void *ptr,
                    size_t size,
                    size_t num);

    int (*close)(os_vio_instance_v0 *vio);
};

struct event_header {
    uint32_t type;
    uint64_t size;
};

//! Mouse button status
struct mouse_event {
    struct event_header h; //!< Event header
    uint32_t button;            //!< Button state
};

struct mouse_move_event {
    struct event_header h; //!< Event header
    vec2f_t pos;       //!< Actual position
};

//! Keyboard event
struct keyboard_event {
    struct event_header h; //!< Event header
    uint32_t keycode; //!< Key code
};

//! Gamepad move event
struct gamepad_move_event {
    struct event_header h; //!< Event header
    uint8_t gamepad_id;         //!< Gamepad id
    uint32_t axis;              //!< Axis id
    vec2f_t position;  //!< Position
};

//! Gamepad button event
struct gamepad_btn_event {
    struct event_header h; //!< Event header
    uint8_t gamepad_id;         //!< Gamepad id
    uint32_t button;            //!< Button state
};

//! Gamepad device event
struct gamepad_device_event {
    struct event_header h; //!< Event header
    uint8_t gamepad_id;         //!< Gamepad id
};

//==============================================================================
// Api
//==============================================================================

struct os_cpu_api_v0 {
    int (*count)();
};

struct os_object_api_v0 {
    void *(*load)(const char *path);

    void (*unload)(void *so);

    void *(*load_function)(void *so,
                           const char *name);
};

struct os_path_v0 {
    //! Get file modified time
    //! \param path File path
    //! \return Modified time
    uint32_t (*file_mtime)(const char *path);

    //! List dir
    //! \param path Dir path
    //! \param recursive Resucrsive list?
    //! \param files Result files
    //! \param allocator Allocator
    void (*list)(const char *path,
                 int recursive,
                 char ***files,
                 uint32_t *count,
                 struct allocator *allocator);

    //! Free list dir array
    //! \param files Files array
    //! \param allocator Allocator
    void (*list_free)(char **files,
                      uint32_t count,
                      struct allocator *allocator);

    //! Create dir path
    //! \param path Path
    //! \return 1 of ok else 0
    int (*make_path)(const char *path);

    //! Get filename from path
    //! \param path Path
    //! \return Filename
    const char *(*filename)(const char *path);

    //! Get file basename (filename without extension)
    //! \param path Path
    //! \param out Out basename
    //! \param size
    void (*basename)(const char *path,
                     char *out,
                     size_t size);

    void (*dir)(char *out,
                size_t size,
                const char *path);

    //! Get file extension
    //! \param path Path
    //! \return file extension
    const char *(*extension)(const char *path);

    //! Join paths
    //! \param allocator Allocator
    //! \param count Path count.
    //! \return Result path len.
    char *(*join)(struct allocator *allocator,
                  uint32_t count,
                  ...);
};

struct os_process_api_v0 {
    int (*exec)(const char *argv);
};

struct os_thread_api_v0 {
    //! Create new thread
    //! \param fce Thread fce
    //! \param name Thread name
    //! \param data Thread data
    //! \return new thread
    os_thread_t *(*create)(thread_fce_t fce,
                           const char *name,
                           void *data);

    //! Kill thread
    //! \param thread thread
    void (*kill)(os_thread_t *thread);

    //! Wait for thread
    //! \param thread Thread
    //! \param status Thread exit status
    void (*wait)(os_thread_t *thread,
                 int *status);

    //! Get id for thread
    //! \param thread Thread
    //! \return ID
    uint64_t (*get_id)(os_thread_t *thread);

    //! Get actual thread id
    //! \return Thread id
    uint64_t (*actual_id)();

    void (*yield)();

    void (*spin_lock)(os_spinlock_t *lock);

    void (*spin_unlock)(os_spinlock_t *lock);
};

struct os_time_api_v0 {
    uint32_t (*ticks)();

    uint64_t (*perf_counter)();

    uint64_t (*perf_freq)();
};

struct os_vio_api_v0 {
    struct os_vio *(*from_file)(const char *path,
                                enum vio_open_mode mode);

    int (*close)(struct os_vio *file);

    int64_t (*seek)(struct os_vio *file,
                    int64_t offset,
                    enum vio_seek whence);

    void (*seek_to_end)(struct os_vio *file);

    int64_t (*skip)(struct os_vio *file,
                    int64_t bytes);

    int64_t (*position)(struct os_vio *file);

    int64_t (*size)(struct os_vio *file);

    size_t (*read)(struct os_vio *file,
                   void *buffer,
                   size_t size,
                   size_t maxnum);

    size_t (*write)(struct os_vio *file,
                    const void *buffer,
                    size_t size,
                    size_t num);

};

struct os_window_api_v0 {
    os_window_t *(*create)(const char *title,
                           enum window_pos x,
                           enum window_pos y,
                           const int32_t width,
                           const int32_t height,
                           enum window_flags flags);

    os_window_t *(*create_from)(void *hndl);

    void (*destroy)(os_window_t *w);

    void (*set_title)(os_window_t *w,
                      const char *title);

    const char *(*get_title)(os_window_t *w);

    void (*update)(os_window_t *w);

    void (*resize)(os_window_t *w,
                   uint32_t width,
                   uint32_t height);

    void (*size)(os_window_t *window,
                 uint32_t *width,
                 uint32_t *height);

    void *(*native_window_ptr)(os_window_t *w);

    void *(*native_display_ptr)(os_window_t *w);
};

//! Machine API V0
struct machine_api_v0 {

    //! Get eventstream begin
    //! \return Begin
    struct event_header *(*event_begin)();

    //! Get eventstream end
    //! \return End
    struct event_header *(*event_end)();

    //! Next event
    //! \param header Actual event header
    //! \return Next event header
    struct event_header *(*event_next)(struct event_header *header);

    //! Is gamepad active?
    //! \param gamepad Gamepad ID
    //! \return 1 if is active else 0
    int (*gamepad_is_active)(int gamepad);

    //!
    //! \param gamepad
    //! \param strength
    //! \param length
    void (*gamepad_play_rumble)(int gamepad,
                                float strength,
                                uint32_t length);

    void (*update)();
};

#ifdef __cplusplus
}
#endif

#endif //CETECH_OS_H