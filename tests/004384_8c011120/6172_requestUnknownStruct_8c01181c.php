<?php

declare(strict_types=1);

use Lhsazevedo\Sh4ObjTest\TestCase;

return new class extends TestCase {
    public function test_1()
    {
        $sizeOfStruct = 8;
        $queueSize = 8;
        $queue = $this->alloc($queueSize * $sizeOfStruct);
        $queueTail = $queue + $queueSize * $sizeOfStruct;

        $this->initUint32($this->addressOf('_var_uknQueueCursor_8c157ab0'), $queue);
        $this->initUint32($this->addressOf('_var_uknQueueTail_8c157ab4'), $queueTail);

        $dirname = $this->allocString('\\MY_DIR');
        $unknownStruct = $this->alloc(4);

        $this->shouldWrite($queue + 0, '\\MY_DIR');
        $this->shouldWrite($queue + 4, $unknownStruct);
        $this->shouldWriteTo('_var_uknQueueCursor_8c157ab0', $queue + $sizeOfStruct);

        $this->call('_requestUnknownStruct_8c01181c')
            ->with($dirname, $unknownStruct)
            ->shouldReturn(1)
            ->run();
    }

    public function test_2()
    {
        $sizeOfStruct = 8;
        $queueSize = 8;
        $queue = $this->alloc($queueSize * $sizeOfStruct);
        $queueTail = $queue + $queueSize * $sizeOfStruct;

        $this->initUint32($this->addressOf('_var_uknQueueCursor_8c157ab0'), $queueTail);
        $this->initUint32($this->addressOf('_var_uknQueueTail_8c157ab4'), $queueTail);

        $dirname = $this->allocString('\\MY_DIR');
        $unknownStruct = $this->alloc(4);


        $this->call('_requestUnknownStruct_8c01181c')
            ->with($dirname, $unknownStruct)
            ->shouldReturn(0)
            ->run();
    }
};
