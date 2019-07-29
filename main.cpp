#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <msgpack.hpp>
#include <ostream>
#include <iostream>
#define REDUNDANCY_LEVEL 1
#include "sysDefs.h"

#define PORT    20001
#define MAXLINE 1024


struct ngps_output
{
    UShort  edge_id = 1;        /* Edge the reference position is on (zero if not localized) */
    ULong   offset = 500;         /* Distance from start of the edge of reference position millimeters */
    ULong   uncertainty = 2;    /* mm */
    SysBool pos_valid = TRUE;
    ULong   speed = 2000;          /* Speed of the train in mm/s */
    SysBool speed_valid = TRUE;
    SysBool gd0 = TRUE;            /* True if the reference position is on GD0 end */
    SysBool reversing = FALSE;      /* gd0 ? offset decreasing : offset increasing */
    SysBool stationary = FALSE;     /* True is train is stationary */
    SShort  accel = 0;          /* mm/s/s */
    UShort  sensorCnt = 1;

    ngps_output() {}

    ngps_output(UShort edgeId, ULong offset, ULong uncertainty, SysBool posValid, ULong speed, SysBool speedValid,
                SysBool gd0, SysBool reversing, SysBool stationary, SShort accel, UShort sensorCnt) :
                    edge_id(edgeId),
                    offset(offset),
                    uncertainty(uncertainty),
                    pos_valid(posValid),
                    speed(speed),
                    speed_valid(speedValid),
                    gd0(gd0),
                    reversing(reversing),
                    stationary(stationary),
                    accel(accel),
                    sensorCnt(sensorCnt) {}

    friend std::ostream &operator<<(std::ostream &os, const ngps_output &output) {
        os << "edge_id: " << output.edge_id << " offset: " << output.offset << " uncertainty: " << output.uncertainty
           << " pos_valid: " << output.pos_valid << " speed: " << output.speed << " speed_valid: " << output.speed_valid
           << " gd0: " << output.gd0 << " reversing: " << output.reversing << " stationary: " << output.stationary
           << " accel: " << output.accel << " sensorCnt: " << output.sensorCnt;
        return os;
    }
};

namespace msgpack {
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
        namespace adaptor {

            template<>
            struct convert<ngps_output> {
                msgpack::object const& operator()(msgpack::object const& o, ngps_output& v) const {
                    if (o.type != msgpack::type::ARRAY) throw msgpack::type_error();
                    if (o.via.array.size != 11) throw msgpack::type_error();
                    v = ngps_output(
                            o.via.array.ptr[0].as<UShort>(),
                            o.via.array.ptr[1].as<ULong>(),
                            o.via.array.ptr[2].as<ULong>(),
                            (SysBool)o.via.array.ptr[3].as<bool>(),
                            o.via.array.ptr[4].as<ULong>(),
                            (SysBool)o.via.array.ptr[5].as<bool>(),
                            (SysBool)o.via.array.ptr[6].as<bool>(),
                            (SysBool)o.via.array.ptr[7].as<bool>(),
                            (SysBool)o.via.array.ptr[8].as<bool>(),
                            o.via.array.ptr[9].as<SShort>(),
                            o.via.array.ptr[10].as<UShort>()
                    );

                    return o;
                }
            };
//Template specialization for msgpacking ngps_output struct
            template<>
            struct pack<ngps_output>
            {
                template <typename Stream>
                packer<Stream>& operator()(msgpack::packer<Stream>& o, ngps_output const& r) const
                {
                    o.pack_array(11);
                    o.pack(r.edge_id);
                    o.pack(r.offset);
                    o.pack(r.uncertainty);
                    o.pack(!!r.pos_valid); //using !! to convert from SysBool to bool
                    o.pack(r.speed);
                    o.pack(!!r.speed_valid);
                    o.pack(!!r.gd0);
                    o.pack(!!r.reversing);
                    o.pack(!!r.stationary);
                    o.pack(r.accel);
                    o.pack(r.sensorCnt);
                    return o;
                }
            };

        } // namespace adaptor
    } // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

int main() {
    int sockfd;
    char buffer[MAXLINE];
    const char * hello = "Hello from client";
    struct sockaddr_in     servaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;
    ngps_output ngps;

    for (int i = 0; i < 10; ++i) {
        ngps.offset += i * 100;
        ngps.speed += i * 1000;

        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, ngps);

        sendto(sockfd, sbuf.data(), sbuf.size(),
               MSG_CONFIRM, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));
        printf("Hello message sent.\n");

        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *) &servaddr,
                     (socklen_t*)&len);
        msgpack::object_handle oh =
                msgpack::unpack(buffer, n);

        msgpack::object obj = oh.get();

        ngps_output rt;
        obj.convert(rt);

        std::cout << rt << std::endl;
        sleep(1);
    }

    close(sockfd);
    return 0;
}