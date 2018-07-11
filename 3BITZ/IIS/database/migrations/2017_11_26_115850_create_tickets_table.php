<?php

use Illuminate\Support\Facades\Schema;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Database\Migrations\Migration;

class CreateTicketsTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('tickets', function (Blueprint $table) {
            $table->increments('id_ticket');
            $table->string('stav');
            $table->string('popis');
            $table->string('name_prideleny')->nullable();
            $table->string('name_vytvoreny');
            $table->foreign('name_prideleny')->references('name')->on('users');
            $table->foreign('name_vytvoreny')->references('name')->on('users');
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('tickets');
    }
}
