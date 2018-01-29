// ==========================================================================
//
// FreeListQueue.h
//
//      Brilib is copyright 2015 under the GNU Public License V.3
//  See the file "COPYING" for details or visit http://www.gnu.org/licenses/gpl.html
//  Created on: Feb 17, 2015
//      Author: brian
//
// --------------------------------------------------------------------------

#ifndef FREELISTQUEUE_H_
#define FREELISTQUEUE_H_

#include <cstdlib>
#include <cstddef>

/*
 *
 */
// ========================================================================
// ------------------------------------------------------------------------
// ========================================================================
/*
 *
 */
template < class T >
class FreeListQueue
{
    private:
        template< class T >
        class QueueNode
        {
            public:
                QueueNode(): Next_( NULL), Data_( NULL ) { }
                virtual
               ~QueueNode();

                void
                Next( QueueNode* next )     { Next_ = next; }
                QueueNode *
                Next( void )                { return( Next_ ); }

                void
                Data( T* item ) { Data_ = item; }
                T*
                Data( void ) { return( Data_ ); }

            protected:

            private:
                QueueNode*  Next_;
                T*          Data_;
        };

    public:
        FreeListQueue() :
        QueueHead_(NULL),
        QueueTail_(NULL),
        QueueSizeEnqueued_(0),
        QueueSizeDequeued_(0),
        FreeHead_(NULL),
        FreeTail_(NULL),
        FreeListSizeEnqueued_(0),
        FreeListSizeDequeued_(0),
        FreeListMinimum_( 100 )
        {
        }

        virtual
        ~FreeListQueue()
        {
            // Walk the Lists and free all remaining Items.
            QueueNode* Node;
            QueueNode* NodeNext;

            // Queue
            Node = QueueHead_;
            QueueHead_ = NULL;
            while( Node != QueueTail_ )
            {
                NodeNext = Node->Next();
                delete Node->Data();    // Delete the Data
                delete Node;            // Delete the Node
                Node = NodeNext;
            }
            delete Node;
            QueueTail_ = NULL;

            // Free List
            Node = FreeHead_;
            FreeHead_ = NULL;
            while( Node != FreeTail_ )
            {
                NodeNext = Node->Next();
                delete Node;            // Delete the Node; no data
                Node = NodeNext;
            }
            delete Node;
            FreeTail_ = NULL;

        }

        void
        Init( void )
        {
            QueueNode* Node = NULL;

            // Insert Dummy to Queue
            Node = new QueueNode();
            QueueHead_ = Node;
            QueueTail_ = Node;

            // Insert Dummy to FreeList
            Node = new QueueNode();
            FreeHead_ = Node;
            FreeTail_ = Node;

        }   // Init

        /* Add an Item to the Queue */
        void
        Enqueue( T* item )
        {
            QueueNode * Node = NULL;

            // Check FreeList for Free Node
            // The FreeHead points to the Dummy Node;
            // if the Tail ALSO points to the dummy Node, the list is empty
            if ( FreeHead_ != FreeTail_ )
            {
                // Dequeue A Node from the Free List
                Node = DeQueueHead( &FreeHead_, FreeListSizeDequeued_ );
            } else {
                Node = new QueueNode();
            }

            // Retain the data and enqueue on the Queue
            Node->Data( item );
            EnQueueTail( &QueueTail_, Node, QueueSizeEnqueued_ );

            size_t      QueueSize = QueueSizeEnqueued_ - QueueSizeDequeued_;
            size_t      FreeListMax = QueueSize >> 3;   // 8% or minimum size.

            // Check to see if the FreeList needs trimming
            while ( FreeListSize() > FreeListMax )
            {
                Node = DeQueueHead( &FreeHead_, FreeListSizeDequeued_ );
                delete Node;
            }
        }   // Enqueue

        /* Remove an item from the Queue */
        T* Dequeue( void )
        {
            QueueNode * Node = NULL;
            T*          ReturnValue;

            // Get the Node
            Node = DeQueueHead( &QueueHead_, QueueSizeDequeued_ );
            ReturnValue = Node->Data();
            Node->Data( NULL );
            EnQueueTail( &FreeTail_, Node, FreeListSizeEnqueued_ );

            return( ReturnValue );
        }   // Dequeue

    protected:
        QueueNode*
        DeQueueHead(  QueueNode** head, size_t& count )
        {

        }

        void
        EnQueueTail(  QueueNode** tail,  QueueNode* node, size_t& count )
        {

        }

        size_t
        FreeListSize( void )
        {
            size_t ReturnValue = FreeListSizeEnqueued_ - FreeListSizeDequeued_;

            return (ReturnValue > FreeListMinimum_) ? ReturnValue : 0;
        }

    private:
        QueueNode*  QueueHead_;
        size_t      QueueSizeDequeued_;

        QueueNode*  QueueTail_;
        size_t      QueueSizeEnqueued_;

        QueueNode*  FreeHead_;
        size_t      FreeListSizeDequeued_;

        QueueNode*  FreeTail_;
        size_t      FreeListSizeEnqueued_;
        size_t      FreeListMinimum_;

};

// ==========================================================================
//                                   END
// ==========================================================================
#endif /* FREELISTQUEUE_H_ */
