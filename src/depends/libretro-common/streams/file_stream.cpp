/* Copyright  (C) 2010-2020 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this file (file_stream.c).
 * ---------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>

#include <streams/file_stream.h>

#include <kodi/Filesystem.h>

struct RFILE
{
   kodi::vfs::CFile* hfile;
   bool error_flag;
};

int64_t filestream_get_size(RFILE *stream)
{
   int64_t output;

   output = stream->hfile->GetLength();

   if (output < 0)
      stream->error_flag = true;

   return output;
}

RFILE* filestream_open(const char *path)
{
   kodi::vfs::CFile* fp = new kodi::vfs::CFile;
   RFILE* output = (RFILE*)malloc(sizeof(RFILE));

   if (!output)
   {
      delete fp;
      return NULL;
   }

   if (!fp->OpenFile(path, 0))
   {
      delete fp;
      free(output);
      return NULL;
   }

   output->error_flag = false;
   output->hfile = fp;
   return output;
}

int filestream_getc(RFILE *stream)
{
   char c = 0;
   if (stream && filestream_read(stream, &c, 1) == 1)
      return (int)(unsigned char)c;
   return EOF;
}

int filestream_eof(RFILE *stream)
{
   return filestream_tell(stream) == filestream_get_size(stream) ? EOF : 0;
}

int64_t filestream_tell(RFILE *stream)
{
   int64_t output;

   output = stream->hfile->GetPosition();

   if (output < 0)
      stream->error_flag = true;

   return output;
}

int64_t filestream_read(RFILE *stream, void *s, int64_t len)
{
   int64_t output;

   output = static_cast<int64_t>(stream->hfile->Read(s, static_cast<size_t>(len)));

   if (output < 0)
      stream->error_flag = true;

   return output;
}

int filestream_error(RFILE *stream)
{
   return (stream && stream->error_flag);
}

int filestream_close(RFILE *stream)
{
   stream->hfile->Close();
   delete stream->hfile;
   free(stream);

   return 0;
}

int64_t filestream_read_file(const char *path, void **buf, int64_t *len)
{
   int64_t ret              = 0;
   int64_t content_buf_size = 0;
   void *content_buf        = NULL;
   RFILE *file              = filestream_open(path);

   if (!file)
   {
      *buf = NULL;
      return 0;
   }

   if ((content_buf_size = filestream_get_size(file)) < 0)
      goto error;

   if (!(content_buf = malloc((size_t)(content_buf_size + 1))))
      goto error;
   if ((int64_t)(uint64_t)(content_buf_size + 1) != (content_buf_size + 1))
      goto error;

   if ((ret = filestream_read(file, content_buf, (int64_t)content_buf_size)) <
         0)
      goto error;

   if (filestream_close(file) != 0)
      free(file);

   *buf    = content_buf;

   /* Allow for easy reading of strings to be safe.
    * Will only work with sane character formatting (Unix). */
   ((char*)content_buf)[ret] = '\0';

   if (len)
      *len = ret;

   return 1;

error:
   if (filestream_close(file) != 0)
      free(file);
   if (content_buf)
      free(content_buf);
   if (len)
      *len = -1;
   *buf = NULL;
   return 0;
}

char *filestream_getline(RFILE *stream)
{
   char *newline_tmp  = NULL;
   size_t cur_size    = 8;
   size_t idx         = 0;
   int in             = 0;
   char *newline      = (char*)malloc(9);

   if (!stream || !newline)
   {
      if (newline)
         free(newline);
      return NULL;
   }

   in = filestream_getc(stream);

   while (in != EOF && in != '\n')
   {
      if (idx == cur_size)
      {
         cur_size    *= 2;

         if (!(newline_tmp = (char*)realloc(newline, cur_size + 1)))
         {
            free(newline);
            return NULL;
         }

         newline     = newline_tmp;
      }

      newline[idx++] = in;
      in             = filestream_getc(stream);
   }

   newline[idx]      = '\0';
   return newline;
}
