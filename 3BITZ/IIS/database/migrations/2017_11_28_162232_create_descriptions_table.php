<?php

use Illuminate\Support\Facades\Schema;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;

class CreateDescriptionsTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('descriptions', function (Blueprint $table) {
            $table->unsignedInteger('id_ticket');
            $table->unsignedInteger('id_bug');

            $table->primary(['id_ticket', 'id_bug']);
            $table->foreign('id_ticket')->references('id_ticket')->on('tickets');
            $table->foreign('id_bug')->references('id_bug')->on('bugs');
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('descriptions');
    }
}
