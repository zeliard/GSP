#pragma once

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/coded_stream.h>

using namespace google;

struct PacketHeader
{
	PacketHeader() : mSize(0), mType(-1) {}
	short mSize;
	short mType;
};

const int HEADER_SIZE = sizeof(PacketHeader);
const int MAX_PKT_TYPE = 1024;

