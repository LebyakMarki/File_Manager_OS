#ifndef ARCHIVES_H
#define ARCHIVES_H

#endif // ARCHIVES_H

#include <archive.h>
#include <archive_entry.h>
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void archive_folder(const QString& arhive_name, const QString& filename) {
    std::string name = "../File_Manager_OS/zip.py ";
    name += filename.toStdString();
    name += " ";
    name += arhive_name.toStdString();
#if defined(WIN32) || defined(_WIN32) || defined(WIN32) && !defined(__CYGWIN)
    std::string command = "python ";
#else
    std::string command = "python3 ";
#endif
    command += name;
    system(command.c_str());
    return;
}


int copy_data(struct archive *ar, struct archive *aw) {
  int r;
  const void *buff;
  size_t size;
  la_int64_t offset;
  for (;;) {
    r = archive_read_data_block(ar, &buff, &size, &offset);
    if (r == ARCHIVE_EOF) {return (ARCHIVE_OK);}
    if (r < ARCHIVE_OK){return (r);}
    r = archive_write_data_block(aw, buff, size, offset);
    if (r < ARCHIVE_OK) {return (r);}
  }
}


int extract(const QString& file_from, const QString& file_to) {
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int r;
    a = archive_read_new();
    archive_read_support_format_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS);
    archive_write_disk_set_standard_lookup(ext);
    const std::string output = file_to.toStdString() + '/';
    if ((r = archive_read_open_filename(a, file_from.toStdString().c_str(), 10240))){return -1;}
    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF) {break;}
        if (r < ARCHIVE_WARN) {return -1;}
        std::string new_entry = output + archive_entry_pathname(entry);
        archive_entry_set_pathname(entry, new_entry.c_str());
        r = archive_write_header(ext, entry);
        if (archive_entry_size(entry) > 0) {
            r = copy_data(a, ext);
            if (r < ARCHIVE_WARN) {return -1;}
        }
        r = archive_write_finish_entry(ext);
        if (r < ARCHIVE_WARN) {return -1;}
    }
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
    return 1111;
}
