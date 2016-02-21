/*-----------------------------------------------------------------------------
**
** Copyright:   Copyright 2016 Rockwell Collins.  All rights reserved.
**
** Source File:  main.cpp
**
** Description:
**
**    This file is an example of how to parse network log raw files.
**
**-----------------------------------------------------------------------------
*/

// system includes
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>

// protobuf lib includes
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>

// app includes
#include "PBNetStat.pb.h"

using namespace google::protobuf::io;
using namespace std;

bool printNLIptableStat(const NL_Message& message)
{
   // always good to validate inputs to a function, right?
   if(message.IsInitialized())
   {
      // loop through all the ipstat items in this message
      for(int i = 0; i < message.ipstat_size(); i++)
      {
         // see if this is a registration message or an update message
         if(message.ipstat(i).has_desc())
         {
            // this has a description so it's a registration message
            cout << setfill(' ') << setw(7) << message.ipstat(i).chain() << "["
                 << setfill(' ') << setw(3) <<  message.ipstat(i).ident()
                 << "], Description: " << message.ipstat(i).desc() << endl;
         }
         else if(message.ipstat(i).has_bytecount() && message.ipstat(i).has_packetcount())
         {
            // this has both byte and packet counts, so it's an update
            cout << setfill(' ') << setw(7) << message.ipstat(i).chain() << "["
                 << setfill(' ') << setw(3) << message.ipstat(i).ident()
                 << "], Bytes: " << setfill(' ') << setw(10) << message.ipstat(i).bytecount()
                 << ", Packets: " << setfill(' ') << setw(10) << message.ipstat(i).packetcount() << endl;
         }
      }
   }
}

bool printNLEthernetStat(const NL_Message& message)
{
	//validates inputs to a function
	if(message.IsInitialized())
	{
		//loop through all ethernetstat items in the message
		for(int i = 0; i < message.ethernetstat_size(); i++)
		{
			//check if a registration or update message
			if(message.ethernetstat(i).has_interface())
			{
				//message has an interface so it is a registration message
				cout << setfill(' ') << setw(7) << message.ethernetstat(i).interface << "["
					 << setfill(' ') << setw(3) << message.ethernetstat(i).ident()
					 << "], Interface name: " << message.ethernetstsat(i).interfaceName() << endl;
			}
			else if(message.ethernetstat(i).has_rxGood() && message.ethernetstat(i).has_rxErrors()
				&& message.ethernetstat(i).has_rxDropped() && message.ethernetstat(i).has_rxOverruns()
				&& message.ethernetstat(i).has_rxFrame() && message.ethernetstat(i).has_rxFrame()
				&& message.ethernetstat(i).has_txGood() && message.ethernetstat(i).has_txErrors()
				&& message.ethernetstat(i).has_txDropped() && message.ethernetstat(i).has_txOverruns()
				&& message.ethernetstat(i).has_txCarrier() && message.ethernetstat(i).has_txCollisions()
				&& message.ethernetstat(i).has_status())
				{
					//has all rx and tx values so this message is an update
					cout << setfill(' ') << setw(7) << message.ethernetstat(i).interface() << "["
						 << setfill(' ') << setw(3) << message.ethernetstat(i).ident()
						 << "], rxGood: " << setfill(' ') << setw(10) << message.ethernetstat(i).rxGood()
						 << ", rxErrors: " << setfill(' ') << setw(10) << message.ethernetstat(i).rxErrors()
						 << ", rxDropped: " << setfill(' ') << setw(10) << message.ethernetstat(i).rxDropped()
						 << ", rxOverruns: " << setfill(' ') << setw(10) << message.ethernetstat(i).rxOverruns()
						 << ", rxFrame: " << setfill(' ') << setw(10) << message.ethernetstat(i).rxFrame()
						 << ", txGood: " << setfill(' ') << setw(10) << message.ethernetstat(i).txGood()
						 << ", txErrors: " << setfill(' ') << setw(10) << message.ethernetstat(i).txErrors()
						 << ", txDropped: " << setfill(' ') << setw(10) << message.ethernetstat(i).txDropped()
						 << ", txOverruns: " << setfill(' ') << setw(10) << message.ethernetstat(i).txOverruns()
						 << ", txCarrier: " << setfill(' ') << setw(10) << message.ethernetstat(i).txCarrier()
						 << ", txCollisions: " << setfill(' ') << setw(10) << message.ethernetstat(i).txCollisions()
						 << ", status: " << setfill(' ') << setw(10) << message.ethernetstat(i).status() << endl;
				}
		}
	}
}

bool printNlMessage(const NL_Message& message)
{
   // always good to validate inputs to a function, right?
   if(message.IsInitialized())
   {
      // source and command are required, so just print them out here
      //    Notice that message.source() returns an integer that maps to the
      //    enum listed in the .proto file, but it's not very "pretty" to
      //    print out. So by calling <element>_Name(...) you can resolve the
      //    enum to a string representation.
      cout << "Source: " << message.Source_Name(message.source()) << endl;
      cout << "Command: " << message.Command_Name(message.command()) << endl;

      // the switch network is optional, so check to see if it exists before printing
      if(message.has_switchstatnetwork())
      {
         // again here we call <element>_Name(...) to resolve to a "pretty" string
         cout << "Network: " << message.Network_Name(message.switchstatnetwork()) << endl;
      }

      // IPTables stats are optional and could have more than one instance, so
      // check to see if there is at least one of them. if so, have it printed
      if(message.ipstat_size() > 0)
      {
         printNLIptableStat(message);
      }

      // same thing for switch stats as IPTables stats
      if(message.switchstat_size() > 0)
      {
         //TODO: Implement a print function for these
      }

      // and again for Ethernet stats
      if(message.ethernetstat_size() > 0)
      {
         printNLEthernetStat(message);
      }
   }
   else
   {
      cerr << "NL Message not initialized" << endl;
      return false;
   }

   // if we got here then all went well
   return true;
}

bool readProtobufMessagesFromFile(const char* const iPath)
{
   cout << "Reading raw file " << iPath << endl;

   int fd = open(iPath, O_RDONLY);
   long int totalSize = 0;

   if(fd >= 0)
   {
      struct stat stbuf;

      // figure out the total file size
      if ((fstat(fd, &stbuf) != 0) || (!S_ISREG(stbuf.st_mode)))
      {
         cerr << "Unable to determine file size" << endl;
         return false;
      }

      totalSize = stbuf.st_size;

      cout << "Total file size is " << totalSize << endl;

      uint32_t msgSize = 0;
      // the zero copy input stream knows how to read files
      ZeroCopyInputStream* raw_input = new FileInputStream(fd);
      // and coded input stream knows how the file is formatted
      CodedInputStream* coded_input  = new CodedInputStream(raw_input);

      // used later to read messages into ... probably could just be one variable,
      // but might be useful to keep the register message around depending on
      // what the goal is
      NL_Message registerMsg;
      NL_Message updateMsg;


      // get the size of the register message (always the first message)
      coded_input->ReadVarint32(&msgSize);

      cout << "Registration message is " << msgSize << " bytes long" << endl;

      // limit the coded stream parser to the size of the registration message
      // this will keep the stream parser from reading into subsequent
      // messages in this file
      CodedInputStream::Limit mLimit = coded_input->PushLimit(msgSize);
      // turn coded stream into registration message
      registerMsg.ParseFromCodedStream(coded_input);
      // pop the coded stream parser limit so we can read another message
      coded_input->PopLimit(mLimit);

      // print the registration message
      printNlMessage(registerMsg);

      // now we need to read all the different stats update messages which
      // will follow the registration message. there is no way at this time
      // to know how many there might be
      while(coded_input->ReadVarint32(&msgSize))
      {
         const void* data;
         int remainingBufferSize;

         // this is used to get the number of bytes left in the current buffer
         if(!coded_input->GetDirectBufferPointer(&data, &remainingBufferSize))
         {
            cerr << "GetDirectBufferPointer failed for some reason" << endl;
            return false;
         }

         // this is used to check to see when we are at the end of the file. due to
         // the way that the coded stream parsing works, it will only read a max of
         // 8k blocks at any given time, so the size variable only represents how
         // much data is left in the current 8k buffer and not how much is left in
         // the total file.
         //
         // to know if we are reading from the last 8k block of the file, we can
         // use the original raw_input stream to see how much of the total file
         // the coded buffer parser has read. once the coded buffer parser has
         // read all the bytes in the total files size, then we know that the 
         // remaining buffer size is also the same as the bytes remaining in the
         // total file.
         //
         // so as long as msgSize is less than the remaining bytes of the file
         // we know that there is another full message to read. but if the next
         // message size is greater than the remaining size in the buffer, then
         // there must be a partial message at the end of the file due to a sync
         // issue during a power loss or something.
         if(totalSize == raw_input->ByteCount() && msgSize > remainingBufferSize)
         {
            cout << "Found a partial message at the end of the raw file. Ignoring it" << endl;
            cout << "File Size: " << totalSize << " Bytes Read: " << raw_input->ByteCount() << endl;
            cout << "Message Size: " << msgSize << " Remaining Size: " << remainingBufferSize << endl;
            break;
         }

         cout << "Update message is " << msgSize << " bytes long" << endl;

         // limit the stream parser so the next read is just this message
         mLimit = coded_input->PushLimit(msgSize);
         updateMsg.ParseFromCodedStream(coded_input);
         //pop the coded stream parser limit so we can read another message
         coded_input->PopLimit(mLimit);

         // now print the update message contents
         printNlMessage(updateMsg);
      }

      // always be good and clean up after yourself so that we don't leak
      // memory and make people think they need more RAM
      close(fd);
      delete coded_input;
      delete raw_input;
   }
   else
   {
      // well that's unexpected ... 
      cerr << "Failed to open file" << endl;
   }
}


int main(int argc, char* argv[])
{
   if(argc == 2)
   {
      readProtobufMessagesFromFile(argv[1]);
      return 0;
   }
   else
   {
      cerr << "Usage: " << argv[0] << " <path to raw file>" << endl;
      return 1;
   }
}


